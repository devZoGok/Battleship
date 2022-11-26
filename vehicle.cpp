#include <cmath>

#include <util.h>
#include <ray.h>
#include <model.h>
#include <quaternion.h>

#include <luaManager.h>

#include "vehicle.h"
#include "pathfinder.h"
#include "map.h"

using namespace gameBase;
using namespace vb01;
using namespace std;

namespace battleship{
	Vehicle::Vehicle(Player *player, int id, Vector3 pos, Quaternion rot) : Unit(player, id, pos, rot){
		initProperties();
	}

	void Vehicle::halt(){
		Unit::halt();
		patrolPointId = 0;
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
		
		pos = pos + dir * speed;
		model->setPosition(pos);
	}

	void Vehicle::initProperties(){
		Unit::initProperties();

		LuaManager *lm = LuaManager::getSingleton();
		string pathBase = GameManager::getSingleton()->getPath() + "Scripts/";
		lm->buildScript(vector<string>{pathBase + "defPaths.lua", pathBase + "vehicleData.lua"});

        maxTurnAngle = lm->getFloatFromTable("maxTurnAngle", vector<Index>{Index(id + 1)});
        speed = lm->getFloatFromTable("speed", vector<Index>{Index(id + 1)});
		anglePrecision = lm->getFloatFromTable("anglePrecision", vector<Index>{Index(id + 1)});
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
				pathPoints.erase(pathPoints.begin());
			else if(type == UnitType::UNDERWATER && fabs(pos.y - pathPoints[0].y) < 0.5 * height && pos.getDistanceFrom(linDest) <= destOffset)
				pathPoints.erase(pathPoints.begin());
		}
	}

	void Vehicle::alignToSurface(){
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

    void Vehicle::move(Order order) {
		navigate(order, 0.5 * Map::getSingleton()->getCellSize().x);

		if(type == UnitType::LAND)
			alignToSurface();

		if(pathPoints.empty())
			removeOrder(0);
    }

	void Vehicle::preparePathpoints(Order order){
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
}
