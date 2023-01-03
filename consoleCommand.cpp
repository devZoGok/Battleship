#include <sstream>
#include <string>
#include <cmath>
#include <vector.h>

#include <stateManager.h>
#include <luaManager.h>

#include "consoleCommand.h"
#include "inGameAppState.h"
#include "player.h"
#include "engineer.h"

using namespace std;
using namespace vb01;
using namespace vb01Gui;
using namespace gameBase;

namespace battleship{
    void ConsoleCommand::execute(string commandStr) {
		vector<string> fullCommand;
		parse(commandStr, fullCommand);

        if (fullCommand[0] == "add-unit") {
			validateAddUnitArguments(fullCommand);
			Vector3 pos = Vector3::VEC_ZERO;
			Quaternion rot = Quaternion::QUAT_W;

			if(fullCommand.size() == 6)
				pos = Vector3(atof(fullCommand[3].c_str()), atof(fullCommand[4].c_str()), atof(fullCommand[5].c_str()));

            executeAddUnit(atoi(fullCommand[1].c_str()), atoi(fullCommand[2].c_str()), pos, rot);
		}
    }

	void ConsoleCommand::parse(string commandStr, vector<string> &fullCommand){
		vector<int> spaceIds;

		for(int i = 0; i < commandStr.length(); i++)
				if(commandStr[i] == ' ')
						spaceIds.push_back(i);

		fullCommand.push_back(commandStr.substr(0, spaceIds[0]));

		for(int i = 0; i < spaceIds.size(); i++){
				bool lastSpace = (i == spaceIds.size() - 1);
				string argument = commandStr.substr(spaceIds[i] + 1, lastSpace ? string::npos : spaceIds[i + 1] - spaceIds[i] - 1);
				fullCommand.push_back(argument);
		}
	}

	//TODO implement rotation argument
	void ConsoleCommand::validateAddUnitArguments(vector<string> &fullCommand){
		//addUnit <playerId> <unitId> [<posX> <posY> <posZ> ]
		if(fullCommand.size() != 3 || fullCommand.size() != 6)
			return;

		int playerId = atoi(fullCommand[1].c_str());

		if(playerId != 0 && playerId != 1)
			return;

		int unitId = atoi(fullCommand[2].c_str());

		LuaManager *lm = LuaManager::getSingleton();

		if(!(0 <= unitId && unitId <= lm->getInt("numUnits")))
			return;
	}

    void ConsoleCommand::executeAddUnit(int playerId, int unitId, Vector3 pos, Quaternion rot) {
        Player* player = Map::getSingleton()->getPlayer(playerId);
		Vehicle *vehicle = nullptr;

		LuaManager *luaManager = LuaManager::getSingleton();
		int unitClass = luaManager->getIntFromTable("unitClass", vector<Index>{Index(unitId + 1)});

		switch((UnitClass)unitClass){
			case UnitClass::ENGINEER:
				vehicle = new Engineer(player, unitId, pos, rot);
				break;
			default:
				vehicle = new Vehicle(player, unitId, pos, rot);
				break;
		}

        player->addUnit(vehicle);
    }
}
