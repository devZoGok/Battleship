#include "gameObjectFactory.h"
#include "player.h"
#include "vehicle.h"
#include "engineer.h"
#include "projectile.h"
#include "resourceDeposit.h"
#include "defConfigs.h"

#include <solUtil.h>

#include <vector>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	Unit* GameObjectFactory::createUnit(Player *player, int id, Vector3 pos, Quaternion rot){
		sol::state_view SOL_LUA_STATE = generateView();
		int unitClass = SOL_LUA_STATE["unitClass"][id + 1];

		switch((UnitClass)unitClass){
			case UnitClass::ENGINEER:
				return new Engineer(player, id, pos, rot);
			default:
				return new Vehicle(player, id, pos, rot);
		}
	}

	Projectile* createProjectile(Unit*, int, vb01::Vector3, vb01::Quaternion){
		return nullptr;
	}
	
	ResourceDeposit* createResourceDeposit(int, vb01::Vector3, vb01::Quaternion){
		return nullptr;
	}
}
