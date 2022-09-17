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
		Vector3 linDest = Vector3(pathPoints[0].x, pos.y, pathPoints[0].z);
		Vector3 destDir = (linDest - pos).norm();

		float angle = dirVec.getAngleBetween(destDir);
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
			else
				pathPoints.erase(pathPoints.begin());

			if(fabs(pos.y - pathPoints[0].y) > destOffset){
				float dist = pos.y - pathPoints[0].y;
				float movementAmmount = (speed > fabs(dist) ? dist : speed);

				if(dist > 0)
					movementAmmount *= -1;

				advance(movementAmmount, MoveDir::UP);
			}
		}

		if(pathPoints.empty())
			removeOrder(0);
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
        Quaternion rotQuat = Quaternion(angle, Vector3(0, 1, 0));
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

	void Unit::generateWeights(u32 **weights, int cellsByDim[3], Vector3 cellSize){
		Map *map = Map::getSingleton();
		int waterbodyId = -1;

		for(int i = 0; i < map->getNumWaterBodies(); i++){
			if(map->getWaterBody(i).isPointWithin(pos)){
				waterbodyId = i;
				break;
			}
		}

		u32 impassibleNodeVal = Pathfinder::getSingleton()->getImpassibleNodeVal();
		int size = cellsByDim[0] * cellsByDim[1] * cellsByDim[2];
		int passVal = 1;

		for(int i = 0; i < size; i++){
			int xId = i % cellsByDim[0];
			int yId = i / (cellsByDim[0] * cellsByDim[2]);
			int zId = (i / cellsByDim[0]) % cellsByDim[2];

			for(int j = 0; j < size; j++){
				int weight = impassibleNodeVal;
				bool adjacent = false;
				
				if(xId == 0 && j - i == 1)
					adjacent = true;
				else if(xId == cellsByDim[0] - 1 && j - i == -1)
					adjacent = true;
				else if(0 < xId && xId < cellsByDim[0] - 1 && abs(j - i) == 1)
					adjacent = true;

				if(yId == 0 && j - i == cellsByDim[0] * cellsByDim[2])
					adjacent = true;
				else if(0 < yId && yId < cellsByDim[1] - 1 && abs(j - i) == cellsByDim[0] * cellsByDim[2])
					adjacent = true;
				else if(yId == cellsByDim[1] - 1 && j - i == -(cellsByDim[0] * cellsByDim[2]))
					adjacent = true;

				if(zId == 0 && j - i == cellsByDim[0])
					adjacent = true;
				else if(zId == cellsByDim[2] - 1 && j - i == -cellsByDim[0])
					adjacent = true;
				else if(0 < zId && zId < cellsByDim[2] - 1 && abs(j - i) == cellsByDim[0])
					adjacent = true;

				if(i == j)
					weight = 0;

				if(adjacent){
					weight = passVal;
					MeshData meshData = map->getTerrainModel()->getChild(0)->getMesh(0)->getMeshBase();
					MeshData::Vertex *verts = meshData.vertices;
					int numVerts = 3 * meshData.numTris;

					for(int k = 0; k < numVerts; k++){
						bool pointWithin = map->isPointWithin(j, verts[k].pos, cellSize, type == UnitType::UNDERWATER);
						WaterBody waterbody;

						if(waterbodyId != -1)
							waterbody = map->getWaterBody(waterbodyId);

						bool impassibleForSeaUnits = (type == UnitType::SEA_LEVEL && pointWithin && waterbodyId != -1 && verts[k].pos.y > waterbody.pos.y);
						bool impassibleForSubs = (type == UnitType::UNDERWATER && pointWithin && waterbodyId != -1);
						bool impassibleForLandUnits = (type == UnitType::LAND && pointWithin && (waterbodyId != -1 && verts[k].pos.y < waterbody.pos.y));

						if(impassibleForSeaUnits || impassibleForSubs || impassibleForLandUnits){
							weight = impassibleNodeVal;
							break;
						}
					}
				}

				weights[i][j] = weight;
			}
		}
	}

	void Unit::addOrder(Order order){
		float eps = getCircleRadius();
		Map *map = Map::getSingleton();

		Vector3 mapSize = map->getSize();
		Vector3 cellSize = Vector3(eps, (type == UnitType::UNDERWATER ? (mapSize.y / height) : 0), eps);

		int cellsByDim[3] = { 
			int(mapSize.x / cellSize.x),
		   	(cellSize.y == 0 ? 1 : cellSize.y),
		   	int(mapSize.z / cellSize.z)
		};
		int numCells = cellsByDim[0] * cellsByDim[1] * cellsByDim[2];
		u32 **weights = new u32*[numCells];

		for(int i = 0; i < numCells; i++)
			weights[i] = new u32[numCells];

		generateWeights(weights, cellsByDim, cellSize);

		Pathfinder *pathfinder = Pathfinder::getSingleton();
		int source = map->getCellId(pos, cellSize);
		int dest = map->getCellId(order.targets[0].pos, cellSize);
		vector<int> path = pathfinder->findPath(weights, numCells, source, dest);
		bool impassibleNodePresent = false;

		pathPoints.clear();

		for(int i = 1; i < path.size(); i++)
			if(!impassibleNodePresent && weights[path[i - 1]][path[i]] == pathfinder->getImpassibleNodeVal())
				impassibleNodePresent = true;

		if(!(impassibleNodePresent || path.empty())){
			for(int p : path)
				pathPoints.push_back(map->getCellPos(p, cellSize));

			orders.push_back(order);
		}

		for(int i = 0; i < numCells; i++)
			delete[] weights[i];

		delete[] weights;
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
