#include <model.h>
#include <quad.h>
#include <material.h>
#include <texture.h>
#include <ray.h>

#include <stateManager.h>
#include <luaManager.h>

#include <glm.hpp>
#include <ext.hpp>

#include "unit.h"
#include "util.h"
#include "inGameAppState.h"
#include "defConfigs.h"
#include "pathfinder.h"

using namespace glm;
using namespace vb01;
using namespace gameBase;
using namespace std;

namespace battleship{
    Unit::Unit(Player *player, vb01::Vector3 pos, int id) {
        this->id = id;
        this->player = player;

		initProperties();
		initModel();
		initSound();
		initUnitStats();
        placeUnit(pos);
    }

	//TODO complete implementation
    Unit::~Unit() {
    }

	void Unit::initProperties(){
		LuaManager *lm = LuaManager::getSingleton();
		string pathBase = GameManager::getSingleton()->getPath() + "Scripts/";
		lm->buildScript(vector<string>{pathBase + "defPaths.lua", pathBase + "unitData.lua"});

        health = lm->getIntFromTable("health", vector<Index>{Index(id + 1)});
		maxHealth = health;

        maxTurnAngle = lm->getFloatFromTable("maxTurnAngle", vector<Index>{Index(id + 1)});
        range = lm->getFloatFromTable("range", vector<Index>{Index(id + 1)});
        lineOfSight = lm->getFloatFromTable("lineOfSight", vector<Index>{Index(id + 1)});
        speed = lm->getFloatFromTable("speed", vector<Index>{Index(id + 1)});
        unitClass = (UnitClass)lm->getIntFromTable("unitClass", vector<Index>{Index(id + 1)});
		anglePrecision = lm->getFloatFromTable("anglePrecision", vector<Index>{Index(id + 1)});
		type = (UnitType)lm->getIntFromTable("unitType", vector<Index>{Index(id + 1)});

		string cornerInd = "unitCornerPoints";

		for(int i = 0; i < 8; i++){
			float x = lm->getFloatFromTable(cornerInd, vector<Index>{Index(id + 1), Index(i + 1), Index("x")});
			float y = lm->getFloatFromTable(cornerInd, vector<Index>{Index(id + 1), Index(i + 1), Index("y")});
			float z = lm->getFloatFromTable(cornerInd, vector<Index>{Index(id + 1), Index(i + 1), Index("z")});
			corners[i] = Vector3(x, y, z);
		}

        width = corners[0].x - corners[1].x;
        height = corners[4].y - corners[0].y;
        length = corners[3].z - corners[0].z;
	}
	
	void Unit::destroyModel(){
		Root::getSingleton()->getRootNode()->dettachChild(model);
		delete model;
	}

	void Unit::initModel(){
		LuaManager *lm = LuaManager::getSingleton();
		string basePath = lm->getStringFromTable("basePath", vector<Index>{Index(id + 1)});
		string meshPath = lm->getStringFromTable("meshPath", vector<Index>{Index(id + 1)});

		model = new Model(basePath + meshPath);
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
	}

	void Unit::destroySound(){
		selectionSfx->stop();
		delete selectionSfx;
		delete selectionSfxBuffer;
	}

	void Unit::initSound(){
		LuaManager *lm = LuaManager::getSingleton();
		string name = lm->getStringFromTable("name", vector<Index>{Index(id + 1)});
        selectionSfxBuffer = new sf::SoundBuffer();
        string p = GameManager::getSingleton()->getPath() + "Sounds/" + name + "s/selection.ogg";

        if(selectionSfxBuffer->loadFromFile(p.c_str()))
            selectionSfx = new sf::Sound(*selectionSfxBuffer);
	}

	void Unit::reinitUnit(){
		destroySound();
		destroyModel();
		initModel();
		initSound();
		initProperties();
		placeUnit(pos);
		orientUnit(rot);
	}

	void Unit::initUnitStats(){
		Root *root = Root::getSingleton();

		hpBackground = new Quad(Vector3(lenHpBar, 10, 0), false);
		string libPath = root->getLibPath();
		Material *hpBackgroundMat = new Material(libPath + "gui");
		hpBackgroundMat->addBoolUniform("texturingEnabled", false);
		hpBackgroundMat->addVec4Uniform("diffuseColor", Vector4(0, 0, 0, 1));
		hpBackground->setMaterial(hpBackgroundMat);
		hpBackgroundNode = new Node();
		hpBackgroundNode->attachMesh(hpBackground);
		Node *guiNode = root->getGuiNode();
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
		hpForeground->setSize(Vector3(health / maxHealth * lenHpBar, 10, 0));
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
                    move(order);
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

	void Unit::navigate(Order order, float destOffset){
		Vector3 hypVec = (pathPoints[0] - pos);
		float hypAngle = hypVec.norm().getAngleBetween(upVec) - PI / 2;
		float offset = hypVec.getLength() * sin(hypAngle);

		Vector3 linDest = pathPoints[0] + upVec * offset;
		Vector3 destDir = (linDest - pos).norm();
		float angle = (destDir != Vector3::VEC_ZERO ? dirVec.getAngleBetween(destDir) : -1);

		if(angle > anglePrecision){
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

			if(fabs(pos.y - pathPoints[0].y) > 0.5 * height){
				float dist = pos.y - pathPoints[0].y;
				float movementAmmount = (speed > fabs(dist) ? dist : speed);

				if(dist > 0)
					movementAmmount *= -1;

				advance(movementAmmount, MoveDir::UP);
			}

			if(type != UnitType::UNDERWATER && pos.getDistanceFrom(linDest) <= destOffset)
				pathPoints.erase(pathPoints.begin());
			else if(type == UnitType::UNDERWATER && fabs(pos.y - pathPoints[0].y) < 0.5 * height && pos.getDistanceFrom(linDest) <= destOffset)
				pathPoints.erase(pathPoints.begin());
		}

		if(pathPoints.empty())
			removeOrder(0);
	}

	void Unit::alignToSurface(){
		Map *map = Map::getSingleton();
		TerrainObject terr = map->getTerrainObject(0);
		vector<Ray::CollisionResult> res;
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

    void Unit::move(Order order) {
		navigate(order, 0.5 * Map::getSingleton()->getCellSize().x);

		if(type == UnitType::LAND)
			alignToSurface();
    }

    void Unit::patrol(Order order) {
        move(order);
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

    void Unit::placeUnit(Vector3 p) {
        model->setPosition(p);
        pos = p;
    }

    void Unit::orientUnit(Quaternion rotQuat){
		model->setOrientation(rotQuat);
		rot = rotQuat;
    }
    
    std::vector<Projectile*> Unit::getProjectiles(){
        std::vector<Projectile*> projectiles;
        InGameAppState *inGameState = ((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

        for(Projectile *p: inGameState->getProjectiles())
            if(p->getUnit() == this)
                projectiles.push_back(p);

        return projectiles;
    }

	void Unit::preparePathpoints(Order order){
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

		if(!(impassibleNodePresent || path.empty()))
			for(int p : path)
				pathPoints.push_back(map->getTerrainObject(srcObjId).cells[p].pos);
	}

	void Unit::addOrder(Order order){
		preparePathpoints(order);

		if(!pathPoints.empty())
			orders.push_back(order);
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

    void Unit::addProjectile(Projectile *p) {
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		((InGameAppState*)stateManager->getAppStateByType((int)AppStateType::IN_GAME_STATE))->addProjectile(p);
	}
}
