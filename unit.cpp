#include <model.h>
#include <quad.h>
#include <material.h>
#include <texture.h>
#include <ray.h>

#include <stateManager.h>

#include <glm.hpp>
#include <ext.hpp>

#include "unit.h"
#include "util.h"
#include "inGameAppState.h"
#include "defConfigs.h"
#include "pathfinder.h"

using namespace glm;
using namespace vb01;
using namespace std;

namespace battleship{
    Unit::Unit(Player *player, vb01::Vector3 pos, int id) {
        this->id = id;
        this->player = player;
		UnitDataManager *udm = UnitDataManager::getSingleton();
        health = udm->getHealth()[id];
        maxTurnAngle = udm->getMaxTurnAngle()[id];
        range = udm->getRange()[id];
        lineOfSight = udm->getLineOfSight()[id];
        speed = udm->getSpeed()[id];
        unitClass = udm->getUnitClass()[id];
        type = udm->getUnitType()[id];
        width = udm->getUnitCornerPoints()[id][0].x - udm->getUnitCornerPoints()[id][1].x;
        height = udm->getUnitCornerPoints()[id][4].y - udm->getUnitCornerPoints()[id][0].y;
        length = udm->getUnitCornerPoints()[id][3].z - udm->getUnitCornerPoints()[id][0].z;

		GameManager *gm = GameManager::getSingleton();
		model = new Model(udm->getBasePath()[id] + udm->getMeshPath()[id]);
		Root *root = Root::getSingleton();
		string libPath = root->getLibPath();

		Material *mat = new Material(libPath + "texture");
		string f[]{configData::DEFAULT_TEXTURE};
        Texture *diffuseTexture = new Texture(f, 1, false);
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", false);
		mat->addTexUniform("textures[0]", diffuseTexture, true);

		model->setMaterial(mat);
		root->getRootNode()->attachChild(model);
        placeUnit(pos);

        selectionSfxBuffer = new sf::SoundBuffer();
        string p = GameManager::getSingleton()->getPath() + "Sounds/" + udm->getName()[id] + "s/selection.ogg";

        if(selectionSfxBuffer->loadFromFile(p.c_str()))
            selectionSfx = new sf::Sound(*selectionSfxBuffer);

		Node *guiNode = root->getGuiNode();

		hpBackground = new Quad(Vector3(lenHpBar, 10, 0), false);
		Material *hpBackgroundMat = new Material(libPath + "gui");
		hpBackgroundMat->addBoolUniform("texturingEnabled", false);
		hpBackgroundMat->addVec4Uniform("diffuseColor", Vector4(0, 0, 0, 1));
		hpBackground->setMaterial(hpBackgroundMat);
		hpBackgroundNode = new Node();
		hpBackgroundNode->attachMesh(hpBackground);
		guiNode->attachChild(hpBackgroundNode);

		hpForeground = new Quad(Vector3(lenHpBar, 10, 0), false);
		Material *hpForegroundMat = new Material(libPath + "gui");
		hpForegroundMat->addBoolUniform("texturingEnabled", false);
		hpForegroundMat->addVec4Uniform("diffuseColor", Vector4(0, 1, 0, 1));
		hpForeground->setMaterial(hpForegroundMat);
		hpForegroundNode = new Node();
		hpForegroundNode->attachMesh(hpForeground);
		guiNode->attachChild(hpForegroundNode);
    }

    Unit::~Unit() {
    }

    void Unit::update() {
		leftVec = model->getGlobalAxis(0);
		upVec = model->getGlobalAxis(1);
		dirVec = model->getGlobalAxis(2);

        if (!orders.empty()){
			LineRenderer *lineRenderer = LineRenderer::getSingleton();
			lineRenderer->toggleVisibility(orders[0].line.id, selected && canDisplayOrderLine());
			lineRenderer->changeLineField(orders[0].line.id, pos, LineRenderer::START);
        }

        executeOrders();

		screenPos = spaceToScreen(pos);
		hpBackgroundNode->setVisible(selected);
		hpForegroundNode->setVisible(selected);

        if (selected)
            displayUnitStats();

        if (health <= 0) 
            blowUp();
    }

    void Unit::blowUp(){
        working = false;
    }

    void Unit::displayUnitStats() {
				float shiftedX = screenPos.x - 0.5 * lenHpBar;
				hpForegroundNode->setPosition(Vector3(shiftedX, screenPos.y, 0));
				hpForeground->setSize(Vector3(health / UnitDataManager::getSingleton()->getHealth()[id] * lenHpBar, 10, 0));
				hpBackgroundNode->setPosition(Vector3(shiftedX, screenPos.y, .1));
    }

    void Unit::executeOrders() {
        if (orders.size() > 0) {
            Order order = orders[0];

            switch (order.type) {
                case Order::TYPE::ATTACK:
                    attack(order);
                    break;
                case Order::TYPE::MOVE:
                    move(order, 0.5 * Map::getSingleton()->getCellSize().x);
                    break;
                case Order::TYPE::PATROL:
                    patrol(order);
                    break;
                case Order::TYPE::LAUNCH:
                    launch(order);
                    break;
                default:
                    break;
            }
        }
    }

    void Unit::setOrder(Order order) {
        while (!orders.empty())
			removeOrder(0);

        addOrder(order);
        orderLineDispTime = getTime();
    }

    void Unit::attack(Order order) {
    }

    void Unit::move(Order order, float destOffset) {
		Vector3 hypVec = (pathPoints[0] - pos);
		float hypAngle = hypVec.norm().getAngleBetween(upVec) - PI / 2;
		float offset = hypVec.getLength() * sin(hypAngle);

		Vector3 linDest = pathPoints[0] + upVec * offset;
		Vector3 destDir = (linDest - pos).norm();
		float angle = (destDir != Vector3::VEC_ZERO ? dirVec.getAngleBetween(destDir) : -1);
		UnitDataManager *udm = UnitDataManager::getSingleton();

		if(angle > udm->getAnglePrecision()[id]){
			float rotSpeed = (maxTurnAngle > angle ? angle : maxTurnAngle); 

			if(leftVec.getAngleBetween(destDir) > PI / 2)
				rotSpeed *= -1;

			turn(rotSpeed);
		}
		else{
			if(pos.getDistanceFrom(linDest) > destOffset){
				float dist = pos.getDistanceFrom(linDest);
				float movementAmmount = (speed > dist ? dist : speed);
				advance(movementAmmount);
			}

			if(fabs(pos.y - pathPoints[0].y) > destOffset){
				float dist = pos.y - pathPoints[0].y;
				float movementAmmount = (speed > fabs(dist) ? dist : speed);

				if(dist > 0)
					movementAmmount *= -1;

				advance(movementAmmount, MoveDir::UP);
			}

			if(pos.getDistanceFrom(type != UnitType::UNDERWATER ? linDest : pathPoints[0]) <= destOffset)
				pathPoints.erase(pathPoints.begin());
		}

		if(pathPoints.empty())
			removeOrder(0);

		vector<Ray::CollisionResult> res;

		if(type == UnitType::LAND){
			Map *map = Map::getSingleton();
			TerrainObject terr = map->getTerrainObject(0);
			Ray::retrieveCollisions(Vector3(pos.x, terr.size.y, pos.z), Vector3(0, -1, 0), terr.node, res);
			Ray::sortResults(res);

			if(!res.empty()){
				placeUnit(res[0].pos);

				float angle = upVec.getAngleBetween(res[0].norm);
				Vector3 axis = upVec.cross(res[0].norm).norm();
				Quaternion rotQuat = Quaternion(angle, axis);
				orientUnit(rotQuat * rot);
			}
		}
    }

    void Unit::patrol(Order order) {
        move(order, UnitDataManager::getSingleton()->getDestinationOffset()[id]);
    }

    void Unit::launch(Order order) {}

    void Unit::advance(float speed, MoveDir moveDir) {
		Vector3 dir;

		switch(moveDir){
			case MoveDir::FORW:
				dir = dirVec;
				break;
			case MoveDir::LEFT:
				dir = leftVec;
				break;
			case MoveDir::UP:
				dir = upVec;
				break;
		}

        pos = pos + dir * speed;
        model->setPosition(pos);
    }

    void Unit::turn(float angle) {
        Quaternion newRot = Quaternion(angle, upVec) * model->getOrientation();
        model->setOrientation(newRot);
		rot = newRot;
    }

    void Unit::halt() {
        while (orders.size() > 0)
            removeOrder(orders.size() - 1);

		patrolPointId = 0;
    }

    float Unit::getCircleRadius() {
        float radius = 0.;

        for (int i = 0; i <= 90 / maxTurnAngle; i++)
            radius += cos(i * (maxTurnAngle / 180 * PI)) * speed;

        return radius;
    }

    void Unit::placeUnit(Vector3 p) {
        model->setPosition(p);
        pos = p;
    }

    void Unit::orientUnit(Quaternion rotQuat){
		model->setOrientation(rotQuat);
		rot = rotQuat;
    }
    
    Vector3 Unit::getCorner(int i) {
        Vector3 corner = UnitDataManager::getSingleton()->getUnitCornerPoints()[id][i];
        return pos + (leftVec * corner.x + upVec * corner.y - dirVec * corner.z);
    }

    std::vector<Projectile*> Unit::getProjectiles(){
        std::vector<Projectile*> projectiles;
        InGameAppState *inGameState = ((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

        for(Projectile *p: inGameState->getProjectiles())
            if(p->getUnit() == this)
                projectiles.push_back(p);

        return projectiles;
    }

	void Unit::addOrder(Order order){
		int srcObjId = 0, destObjId = 0;
		Map *map = Map::getSingleton();

		for(int i = 1; i < map->getNumTerrainObjects(); i++){
			if(map->isPointWithinTerrainObject(pos, i))
				srcObjId = i;

			if(map->isPointWithinTerrainObject(order.targets[0].pos, i))
				destObjId = i;
		}

		if(srcObjId != destObjId)
			return;

		int source = map->getCellId(pos, srcObjId);
		int dest = map->getCellId(order.targets[0].pos, destObjId);

		Pathfinder *pathfinder = Pathfinder::getSingleton();
		u32 **weights = map->getTerrainObject(srcObjId).weights;
		vector<int> path = pathfinder->findPath(weights, map->getTerrainObject(srcObjId).numCells, source, dest);

		bool impassibleNodePresent = false;
		pathPoints.clear();

		for(int i = 1; i < path.size(); i++)
			if(!impassibleNodePresent && weights[path[i - 1]][path[i]] == pathfinder->getImpassibleNodeVal())
				impassibleNodePresent = true;

		if(!(impassibleNodePresent || path.empty())){
			for(int p : path)
				pathPoints.push_back(map->getTerrainObject(srcObjId).cells[p].pos);

			orders.push_back(order);
		}
	}
    
    void Unit::removeOrder(int id) {
		LineRenderer::getSingleton()->removeLine(orders[id].line.id);
        orders.erase(orders.begin() + id);
    }

    void Unit::toggleSelection(bool selection) {
        selected = selection;

        if(selection && selectionSfx)
            selectionSfx->play();

        orderLineDispTime = getTime();
    }

    void Unit::addProjectile(Projectile *p) {((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE))->addProjectile(p);}
}
