#include "cruiseMissile.h"
#include "player.h"
#include "unit.h"
#include "map.h"

#include <util.h>

#include <cmath>

namespace battleship{
	using namespace std;
	using namespace vb01;

	CruiseMissile::CruiseMissile(Unit *unit, Vector3 tp, Vector3 pos, Quaternion rot) : Projectile(unit, 0, pos, rot), targetPoint(tp), flightStage(FlightStage::ASCENT){
		Vector3 unitDir = unit->getDirVec(); 
		Vector3 leftDir = unit->getLeftVec();
		Vector3 targDir = (Vector3(targetPoint.x, pos.y, targetPoint.z) - pos).norm();
		
		bool left = (leftDir.getAngleBetween(targDir) < PI / 2);
		float angle = unitDir.getAngleBetween(targDir) * (left ? 1 : -1);
		orientAt(Quaternion(angle, Vector3::VEC_J) * rot);
	}

	//TODO clean up method code
	void CruiseMissile::pitch(float rotAngle){
		float minHeight = 20;

		if(flightStage == FlightStage::ASCENT && pos.y - initPos.y > minHeight){
			Vector3 horDir = Vector3(dirVec.x, 0, dirVec.z).norm();
			float angleToVertDir = dirVec.getAngleBetween(horDir);
			Quaternion rotQuat = Quaternion(angleToVertDir > rotAngle ? rotAngle : angleToVertDir, leftVec) * rot; 
			orientAt(rotQuat);

			if(dirVec.y <= 0)
				flightStage = FlightStage::CRUISE;
		}
		else if(flightStage == FlightStage::DESCENT){
			float angleFromHorDir = dirVec.getAngleBetween(-Vector3::VEC_J);
			float angle = (angleFromHorDir > rotAngle ? rotAngle : angleFromHorDir);
			Vector3 targDir = (Vector3(targetPoint.x, initPos.y, targetPoint.z) - initPos).norm();

			if(dirVec.getAngleBetween(targDir) < PI / 2)
				orientAt(Quaternion(angle, leftVec) * rot);
		}
	}

	void CruiseMissile::cruise(){
		float minDist = 6;
		float initDist = Vector3(targetPoint.x, initPos.y, targetPoint.z).getDistanceFrom(initPos);
		
		if(pos.getDistanceFrom(Vector3(targetPoint.x, pos.y, targetPoint.z)) < minDist)
			flightStage = FlightStage::DESCENT;
	}

	void CruiseMissile::checkSurfaceCollision(){
		Map *map = Map::getSingleton();
		int cellId = map->getCellId(pos, false);

		if((pos + dirVec * rayLength).y <= map->getCells()[cellId].pos.y)
			explode();
	}

	void CruiseMissile::update(){
		GameObject::update();
		placeAt(pos + dirVec * speed);

		switch(flightStage){
			case FlightStage::ASCENT:
			case FlightStage::DESCENT:
				pitch(.1);
				break;
			case FlightStage::CRUISE:
				cruise();
				break;
		}

		if(!exploded && flightStage == FlightStage::DESCENT){
		   	checkSurfaceCollision();
			if(exploded) return;
		   	checkUnitCollision();
		}
	}
}
