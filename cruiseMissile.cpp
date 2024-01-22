#include "cruiseMissile.h"
#include "player.h"
#include "unit.h"
#include "map.h"
#include "game.h"

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

	void CruiseMissile::pitch(float rotAngle, Vector3 compVec){
		float minHeight = 20;
		float angleToCompVec = dirVec.getAngleBetween(compVec);
		float angle = (angleToCompVec > rotAngle ? rotAngle : angleToCompVec);

		if(flightStage == FlightStage::ASCENT && pos.y - initPos.y > minHeight){
			orientAt(Quaternion(angle, leftVec) * rot);
			if(dirVec.y <= 0) flightStage = FlightStage::CRUISE;
		}
		else if(flightStage == FlightStage::DESCENT){
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

		if((pos + dirVec * rayLength).y <= map->getCells()[cellId].pos.y){
			exploded = true;
			Game::getSingleton()->explode(pos, explosionDamage, explosionRadius, explosionSfx);
			player->removeProjectile(this);
		}
	}

	void CruiseMissile::update(){
		GameObject::update();
		placeAt(pos + dirVec * speed);
		float rotAngle = .1;

		switch(flightStage){
			case FlightStage::ASCENT:
				pitch(rotAngle, Vector3(dirVec.x, 0, dirVec.z).norm());
				break;
			case FlightStage::DESCENT:
				pitch(rotAngle, -Vector3::VEC_J);
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
