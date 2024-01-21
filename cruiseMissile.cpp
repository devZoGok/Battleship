#include "cruiseMissile.h"
#include "unit.h"

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

	void CruiseMissile::pitch(float rotAngle){
		Vector3 horDir = Vector3(dirVec.x, 0, dirVec.z).norm();
		float angleToVertDir = dirVec.getAngleBetween(horDir);
		Quaternion rotQuat = Quaternion(angleToVertDir > rotAngle ? rotAngle : angleToVertDir, leftVec) * rot; 
		float minHeight = 20;

		if(flightStage == FlightStage::ASCENT && pos.y - initPos.y > minHeight){
			orientAt(rotQuat);

			if(dirVec.y <= 0)
				flightStage = FlightStage::CRUISE;
		}
		else if(flightStage == FlightStage::DESCENT && dirVec.getAngleBetween((Vector3(targetPoint.x, initPos.y, targetPoint.z) - initPos).norm()) < PI / 2)
			orientAt(rotQuat);
	}

	void CruiseMissile::cruise(){
		float minDist = 3;
		
		if(pos.getDistanceFrom(Vector3(targetPoint.x, pos.y, targetPoint.z)) < minDist)
			flightStage = FlightStage::DESCENT;
	}

	void CruiseMissile::update(){
		GameObject::update();
		placeAt(pos + dirVec * speed);

		float rotAngle = .1, eps = .0001;

		switch(flightStage){
			case FlightStage::ASCENT:
			case FlightStage::DESCENT:
				pitch(rotAngle);
				break;
			case FlightStage::CRUISE:
				cruise();
				break;
		}

		//if(!exploded && flightStage == FlightStage::DESCENT) checkForCollision();
	}
}
