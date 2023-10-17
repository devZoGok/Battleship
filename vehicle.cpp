#include <cmath>

#include <solUtil.h>

#include <util.h>
#include <ray.h>
#include <box.h>
#include <model.h>
#include <quaternion.h>

#include "vehicle.h"
#include "pathfinder.h"
#include "garrisonable.h"
#include "map.h"

using namespace gameBase;
using namespace vb01;
using namespace std;

namespace battleship{
	Vehicle::Vehicle(Player *player, int id, Vector3 pos, Quaternion rot) : Unit(player, id, pos, rot){
		initProperties();

		debugMat = new Material(Root::getSingleton()->getLibPath() + "texture");
		debugMat->addBoolUniform("lightingEnabled", false);
		debugMat->addBoolUniform("texturingEnabled", false);
		debugMat->addVec4Uniform("diffuseColor", Vector4::VEC_IJKL);
	}

	Vehicle::~Vehicle(){
		removeAllPathpoints();
		delete debugMat;

		Unit::~Unit();
	}

	void Vehicle::halt(){
		Unit::halt();
		removeAllPathpoints();

		patrolPointId = 0;
		pursuingTarget = false;
	}

	void Vehicle::addOrder(Order order){
		preparePathpoints(order);

		if(!pathPoints.empty())
			orders.push_back(order);
	}

    void Vehicle::turn(float angle) {
        Quaternion newRot = Quaternion(angle, upVec) * model->getOrientation();
        model->setOrientation(newRot);
		rot = newRot;
    }

	void Vehicle::advance(float speed, MoveDir moveDir) {
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
		
		placeAt(pos + dir * speed);
	}

	void Vehicle::initProperties(){
		Unit::initProperties();

		sol::state_view SOL_LUA_STATE = generateView();
        maxTurnAngle = SOL_LUA_STATE["maxTurnAngle"][id + 1];
        speed = SOL_LUA_STATE["speed"][id + 1];
		anglePrecision = SOL_LUA_STATE["anglePrecision"][id + 1];
	}

	void Vehicle::navigate(Order order, float destOffset){
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
				removePathpoint();
			else if(type == UnitType::UNDERWATER && fabs(pos.y - pathPoints[0].y) < 0.5 * height && pos.getDistanceFrom(linDest) <= destOffset)
				removePathpoint();
		}
	}

	void Vehicle::alignToSurface(){
		/*
		Map *map = Map::getSingleton();
		TerrainObject terr = map->getTerrainObject(0);
		vector<Ray::CollisionResult> res;
		Ray::retrieveCollisions(Vector3(pos.x, terr.size.y, pos.z), Vector3(0, -1, 0), terr.node, res);
		Ray::sortResults(res);
		
		if(!res.empty()){
			placeAt(res[0].pos);
		
			float angle = upVec.getAngleBetween(res[0].norm);
			Vector3 axis = upVec.cross(res[0].norm).norm();
			Quaternion rotQuat = Quaternion(angle, axis);
			orientAt(rotQuat * rot);
		}
		*/
	}

    void Vehicle::move(Order order) {
		navigate(order, 0.5 * Map::getSingleton()->getCellSize().x);

		if(type == UnitType::LAND)
			alignToSurface();

		if(pathPoints.empty())
			removeOrder(0);
    }

	void Vehicle::garrison(Garrisonable *garrisonable){
	}

	//TODO add hover unit type
	//TODO cleanup debug pathpoint removal
	void Vehicle::preparePathpoints(Order order){
		removeAllPathpoints();

		Map *map = Map::getSingleton();
		vector<Map::Cell> &cells = map->getCells();

		int source = map->getCellId(pos);

		Order::Target targ = order.targets[0];
		int dest = map->getCellId(targ.unit ? targ.unit->getPos() : targ.pos);

		if(type == UnitType::UNDERWATER || type == UnitType::SEA_LEVEL || type == UnitType::LAND){
			Map::Cell::Type cellType = (type == UnitType::LAND ? Map::Cell::LAND : Map::Cell::WATER);
			bool canReach = (cells[source].type == cellType && cells[dest].type == cellType);

			if(!canReach) return;
		}

		Pathfinder *pathfinder = Pathfinder::getSingleton();
		vector<int> path = pathfinder->findPath(cells, source, dest, (int)type);
		Node *rootNode = Root::getSingleton()->getRootNode();

		for(int p : path){
			pathPoints.push_back(cells[p].pos);

			Box *b = new Box(Vector3::VEC_IJK);
			b->setMaterial(debugMat);
			Node *n = new Node(cells[p].pos);
			n->attachMesh(b);
			rootNode->attachChild(n);
			debugPathPoints.push_back(n);
		}
	}

	void Vehicle::removePathpoint(int i){
		Node *rootNode = Root::getSingleton()->getRootNode();
		Node *debugPathPointNode = debugPathPoints[i];
		rootNode->dettachChild(debugPathPointNode);

		Mesh *mesh = debugPathPointNode->getMesh(0);
		mesh->setMaterial(nullptr);
		debugPathPoints.erase(debugPathPoints.begin() + i);
		delete debugPathPointNode;

		pathPoints.erase(pathPoints.begin() + i);

		if(pathPoints.empty())
			pursuingTarget = false;
	}

	void Vehicle::removeAllPathpoints(){
		while(!pathPoints.empty())
			removePathpoint();
	}

	void Vehicle::attack(Order order){
		Order::Target target = order.targets[0];
		float distToTarg = pos.getDistanceFrom(target.unit ? target.unit->getPos() : target.pos);
		float minDist = range;

		if(distToTarg > minDist){
			if(!pursuingTarget){
				preparePathpoints(order);
				pursuingTarget = true;
			}

			navigate(order, .5 *  Map::getSingleton()->getCellSize().x);
		}
		else{
			pursuingTarget = false;

			if(getTime() - lastFireTime > rateOfFire){
				fire();

				lastFireTime = getTime();
			}
		}
	}

	void Vehicle::fire(){
		fireSfx->play();

		Unit *targetUnit = orders[0].targets[0].unit;

		if(targetUnit)
			targetUnit->takeDamage(1);
	}
}
