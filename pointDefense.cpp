#include "pointDefense.h"

#include <string>

namespace battleship{
	using namespace vb01;
	using namespace std;
	using namespace gameBase;

	PointDefense::PointDefense(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus, Unit::State state) : Structure(player, id, pos, rot, buildStatus, state){
		turretDir = dirVec;
	}

	void PointDefense::attack(Order order){
		if(!isComplete()) return;

		Unit *targUnit = order.targets[0].unit;
		Vector3 targDir = (targUnit ? targUnit->getPos() : order.targets[0].pos) - pos;

		if(targDir.getLength() > weapons[0]->getMaxRange()){
			removeOrder(0);
			return;
		}

		float angleToTarg = targDir.norm().getAngleBetween(turretDir);
		
		if(angleToTarg > .05)
			rotateTurret(calculateRotation(targDir.norm(), angleToTarg, .1));
		else
			weapons[0]->fire(order);

		Unit::attack(order);
	}

	void PointDefense::rotateTurret(float angle){
		string nodeName = generateView()[GameObject::getGameObjTableName()][id + 1]["turretNode"];
		Node *turretNode = model->findDescendant(nodeName, true);
		Quaternion rotQuat = Quaternion(angle, Vector3::VEC_J);
		turretNode->setOrientation(rotQuat * turretNode->getOrientation());
		turretDir = rotQuat * turretDir;
	}
}
