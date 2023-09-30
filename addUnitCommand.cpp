#include <solUtil.h>
#include <stateManager.h>

#include "addUnitCommand.h"
#include "console.h"
#include "addUnitCommand.h"
#include "inGameAppState.h"
#include "player.h"
#include "unitFactory.h"

namespace battleship{
	using namespace gameBase;
	using namespace vb01;

	void AddUnitCommand::validate(){
		if(!(arguments.size() == 2 || arguments.size() == 5 || arguments.size() == 9))
			return;

		playerId = atoi(arguments[0].c_str());

		if(playerId != 0 && playerId != 1)
			return;

		unitId = atoi(arguments[1].c_str());

		if(!(0 <= unitId && unitId <= (int)generateView()["numUnits"]))
			return;

		if(arguments.size() >= 5){
			float x = atoi(arguments[2].c_str());
			float y = atoi(arguments[3].c_str());
			float z = atoi(arguments[4].c_str());
			pos = Vector3(x, y, z);
		}
		else if(2 < arguments.size() && arguments.size() < 5)
			return;

		if(arguments.size() == 9){
			float w = atoi(arguments[5].c_str());
			float x = atoi(arguments[6].c_str());
			float y = atoi(arguments[7].c_str());
			float z = atoi(arguments[8].c_str());
			rot = Quaternion(w, x, y, z);
		}
		else if(5 < arguments.size() && arguments.size() < 9)
			return;
	}

	void AddUnitCommand::addUnit(){
        Player* player = Map::getSingleton()->getPlayer(playerId);
        player->addUnit(UnitFactory::createUnit(player, unitId, pos, rot));
	}

	void AddUnitCommand::execute(){
		AbstractCommand::handle();
		validate();
		addUnit();
	}
}
