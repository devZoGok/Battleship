#include "pointDefense.h"

namespace battleship{
	using namespace vb01;

	PointDefense::PointDefense(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus) : Structure(player, id, pos, rot, buildStatus){
		turretDir = dirVec;
	}

	void PointDefense::attack(Order order){
		Unit *targUnit = order.targets[0].unit;
		Vector3 targDir = (targUnit ? targUnit->getPos() : order.targets[0].pos) - pos;

		if(targDir.getLength() > range){
			removeOrder(0);
			return;
		}

		float angleToTarg = targDir.norm().getAngleBetween(turretDir);
		
		if(angleToTarg > .05)
			rotateTurret(calculateRotation(targDir.norm(), angleToTarg, .1));
		else if(canFire())
			fire();

		Unit::attack(order);
	}

	void PointDefense::rotateTurret(float angle){
		Node *turretNode = model->getChild(0)->getChild(0);
		Quaternion rotQuat = Quaternion(angle, Vector3::VEC_J);
		turretNode->setOrientation(rotQuat * turretNode->getOrientation());
		turretDir = rotQuat * turretDir;
	}
}
