#include "gameObjectFactory.h"
#include "player.h"
#include "factory.h"
#include "engineer.h"
#include "transport.h"
#include "projectile.h"
#include "resourceDeposit.h"
#include "pointDefense.h"
#include "defConfigs.h"

#include <solUtil.h>

#include <vector>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	Unit* GameObjectFactory::createUnit(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus){
		sol::state_view SOL_LUA_STATE = generateView();
		int unitClass = SOL_LUA_STATE["units"]["unitClass"][id + 1];

		switch((UnitClass)unitClass){
			case UnitClass::ENGINEER:
				return new Engineer(player, id, pos, rot);
			case UnitClass::TRANSPORT:
				return new Transport(player, id, pos, rot);
			case UnitClass::LAND_FACTORY:
			case UnitClass::NAVAL_FACTORY:
				return new Factory(player, id, pos, rot, buildStatus);
			case UnitClass::POINT_DEFENSE:
				return new PointDefense(player, id, pos, rot, buildStatus);
			case UnitClass::MARKET:
			case UnitClass::LAB:
				return new Structure(player, id, pos, rot, buildStatus);
			default:
				return new Vehicle(player, id, pos, rot);
		}
	}

	Projectile* GameObjectFactory::createProjectile(Unit *unti, int id, Vector3 pos, Quaternion rot){
		return nullptr;
	}
	
	ResourceDeposit* GameObjectFactory::createResourceDeposit(Player *player, int id, Vector3 pos, Quaternion rot){
		return new ResourceDeposit(player, id, pos, rot);
	}
}
