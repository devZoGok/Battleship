#include "structure.h"

#include <node.h>

using namespace gameBase;
using namespace vb01;
using namespace std;

namespace battleship{
	Structure::Structure(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus) : Unit(player, id, pos, rot){
		this->buildStatus = buildStatus;

		Vector2 size = Vector2(lenHpBar, 10);
		buildStatusBackground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 0, 1));
		buildStatusForeground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 1, 1));
	}

	Structure::~Structure(){
		removeBar(buildStatusForeground);
		removeBar(buildStatusBackground);
		Unit::~Unit();
	}

	void Structure::update(){
		Unit::update();

		if(selected && buildStatus < 100)
			Unit::displayUnitStats(buildStatusForeground, buildStatusBackground, buildStatus, 100, Vector2(0, -10));
		else{
			buildStatusBackground->setVisible(false);
			buildStatusForeground->setVisible(false);
		}
	}
}
