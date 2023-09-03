#include "unitFactory.h"
#include "player.h"
#include "vehicle.h"
#include "engineer.h"

#include <solUtil.h>

#include <vector>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	Unit* UnitFactory::createUnit(Player *player, int id, Vector3 pos, Quaternion rot){
		sol::state_view SOL_LUA_STATE = generateView();
		int unitClass = SOL_LUA_STATE["unitClass"][id + 1];
		Unit *unit = nullptr;

		switch((UnitClass)unitClass){
			case UnitClass::ENGINEER:
				unit = new Engineer(player, id, pos, rot);
				break;
			default:
				unit = new Vehicle(player, id, pos, rot);
				break;
		}

		return unit;
	}
}
