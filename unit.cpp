#include <model.h>
#include <quad.h>
#include <material.h>
#include <texture.h>

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

        if (selected) {
            displayUnitStats();
        }

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
                    move(order, UnitDataManager::getSingleton()->getDestinationOffset()[id]);
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
        float movementAmmount, radius = getCircleRadius(), angle = 0.;
		int targetId = (order.type == Order::TYPE::PATROL ? getNextPatrolPointId(order.targets.size()) : 0);
        Vector3 dest = pathPoints[0];
        dest.y = pos.y;
        Vector3 center;

        if (leftVec.getAngleBetween(dest - pos) < PI / 2) {
            moveDir = MoveDir::LEFT;
            center = pos + leftVec*radius;
        } else if (-leftVec.getAngleBetween(dest - pos) < PI / 2) {
            moveDir = MoveDir::RIGHT;
            center = pos - leftVec*radius;
        }

        bool withinCircle = center.getDistanceFrom(dest) < radius ? true : false;
		float anglePrecision = UnitDataManager::getSingleton()->getAnglePrecision()[id];

        if (moveDir != MoveDir::FORWARD) {
            if (withinCircle)
                angle = dirVec.getAngleBetween(center - dest) / PI * 180;
            else
                angle = dirVec.getAngleBetween(dest - pos) / PI * 180;

            angle = isnan(angle) ? 0. : angle;

            if (angle > anglePrecision) {
                float rotAngle = maxTurnAngle > angle ? angle : maxTurnAngle;
                angle -= rotAngle;
                turn(moveDir == MoveDir::RIGHT ? -rotAngle : rotAngle);
            }
        }

        if (angle > anglePrecision)
            movementAmmount = speed;
        else {
            if (withinCircle) {
                float inDest = ((center - dest).norm() * radius - (center - dest)).getLength();
                movementAmmount = speed > inDest ? inDest : speed;
            } else
                movementAmmount = speed > pos.getDistanceFrom(dest) ? pos.getDistanceFrom(dest) : speed;
        }

        advance(movementAmmount);

        if (pos.getDistanceFrom(dest) <= destOffset){
			pathPoints.erase(pathPoints.begin());

			if(pathPoints.empty() && orders[0].type != Order::TYPE::ATTACK) {
        		moveDir = MoveDir::FORWARD;

				if(orders[0].type == Order::TYPE::MOVE)
        			removeOrder(0);
				else if(orders[0].type == Order::TYPE::PATROL)
					patrolPointId = targetId;
			}
        }
    }

    void Unit::patrol(Order order) {
        move(order, UnitDataManager::getSingleton()->getDestinationOffset()[id]);
    }

    void Unit::launch(Order order) {}

    void Unit::advance(float speed) {
        pos = pos + dirVec * speed;
        model->setPosition(pos);
    }

    void Unit::turn(float angle) {
        Quaternion rotQuat = Quaternion(PI / 180 * angle, Vector3(0, 1, 0));
        model->setOrientation(rotQuat * model->getOrientation());
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

    void Unit::orientUnit(Vector3 orientVec){
        Vector3 orientVecProj = Vector3(orientVec.x, 0, orientVec.z).norm();
        float projAngle = orientVec.getAngleBetween(orientVecProj);
        float rotAngle = Vector3(0,0,-1).getAngleBetween(orientVecProj);

        rotAngle *= (orientVec.x < 0 ? 1 : -1) / PI * 180;
        projAngle *= (orientVec.y > 0 ? 1 : -1) / PI * 180;
        dirVec = orientVecProj;
        upVec = Vector3(0,1,0);
        leftVec = Quaternion(-PI / 2, upVec) * dirVec;
        Quaternion rotQuat = Quaternion(projAngle / 180 * PI, leftVec);
        dirVec = rotQuat * dirVec;
        upVec = rotQuat * upVec;
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

	void Unit::generateWeights(u32 **weights, int size){
		Map *map = Map::getSingleton();
		int waterbodyId = -1;

		if(type == UnitType::UNDERWATER || type == UnitType::SEA_LEVEL)
			for(int i = 0; i < map->getNumWaterBodies(); i++){
				WaterBody waterBody = map->getWaterBody(i);
				Vector3 p = waterBody.pos;
				Vector2 s = waterBody.size;

				if((waterBody.rect && fabs(pos.x - p.x) < s.x && fabs(pos.z - p.z) < s.y) ||
					   	(!waterBody.rect && Vector2(pos.x, pos.z).getDistanceFrom(Vector2(p.x, p.z)) < s.x)){
					waterbodyId = i;
					break;
				}
			}

		u32 impassibleNodeVal = Pathfinder::getSingleton()->getImpassibleNodeVal();

		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				int weight = (abs(i - j) == 1 ? 1 : impassibleNodeVal);
				weights[i][j] = weight;
			}
		}
	}

	void Unit::addOrder(Order order){
		float eps = getCircleRadius();
		Map *map = Map::getSingleton();

		Vector3 mapSize = map->getSize();
		Vector3 cellSize = Vector3(eps, (type == UnitType::UNDERWATER ? int(mapSize.y / height) : 0), eps);

		int numCells = 
			int(mapSize.x / cellSize.x) *
		   	(cellSize.y == 0 ? 1 : int(mapSize.y / cellSize.y)) *
		   	int(mapSize.z / cellSize.z);
		u32 **weights = new u32*[numCells];

		for(int i = 0; i < numCells; i++)
			weights[i] = new u32[numCells];

		generateWeights(weights, numCells);

		Pathfinder *pathfinder = Pathfinder::getSingleton();
		vector<int> path = pathfinder->findPath(weights,
			   	numCells,
			   	map->getCellId(pos, cellSize),
			   	map->getCellId(order.targets[0].pos, cellSize));

		for(int i = 0; i < numCells; i++)
			delete[] weights[i];

		delete[] weights;

		pathPoints.clear();

		for(int p : path)
			pathPoints.push_back(map->getCellPos(p, cellSize));

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

    void Unit::addProjectile(Projectile *p) {((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE))->addProjectile(p);}
}
