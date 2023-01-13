#include "unitFactory.h"
#include "player.h"
#include "vehicle.h"
#include "engineer.h"

#include <luaManager.h>

#include <vector>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	Unit* UnitFactory::createUnit(Player *player, int id, Vector3 pos, Quaternion rot){
		Unit *unit = nullptr;

		LuaManager *luaManager = LuaManager::getSingleton();
		int unitClass = luaManager->getIntFromTable("unitClass", vector<Index>{Index(id + 1)});

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
