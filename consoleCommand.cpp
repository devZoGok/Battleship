#include <sstream>
#include <string>
#include <cmath>
#include <vector.h>

#include <stateManager.h>

#include "consoleCommand.h"
#include "unitData.h"
#include "aircraftCarrier.h"
#include "cruiser.h"
#include "destroyer.h"
#include "submarine.h"
#include "demoJet.h"
#include "missileJet.h"
#include "inGameAppState.h"
#include "unitData.h"

using namespace std;
using namespace vb01;
using namespace vb01Gui;

namespace battleship{
		using namespace unitData;

    void ConsoleCommand::execute(string commandStr) {
				vector<string> fullCommand;
				parse(commandStr, fullCommand);

        if (fullCommand[0] == "add-unit") {
						validateAddUnitArguments(fullCommand);
            executeAddUnit(atoi(fullCommand[1].c_str()), atoi(fullCommand[2].c_str()));
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

		void ConsoleCommand::validateAddUnitArguments(vector<string> &fullCommand){
				//addUnit <playerId> <unitId> [<posX> <posY> <posZ> ]
				if(fullCommand.size() != 3 || fullCommand.size() != 6)
						return;

				int playerId = atoi(fullCommand[1].c_str());

				if(playerId != 0 && playerId != 1)
					return;

				int unitId = atoi(fullCommand[2].c_str());

				if(!(0 <= unitId && unitId <= unitData::numberOfUnits))
					return;
		}

    void ConsoleCommand::executeAddUnit(int playerId, int unitId) {
				StateManager *stateManager = GameManager::getSingleton()->getStateManager();
				InGameAppState *inGameState = (InGameAppState*)stateManager->getAppStateByType(AppStateType::IN_GAME_STATE);
        vector<Player*> players = inGameState->getPlayers();
        Vector3 pos(0, 0, 0);
        Unit *u = nullptr;

        switch (unitType[unitId]) {
            case UNIT_TYPE::VESSEL:
                u = new Vessel(players[playerId], pos, unitId);
                break;
            case UNIT_TYPE::DESTROYER:
                u = new Destroyer(players[playerId], pos, unitId);
                break;
            case UNIT_TYPE::CRUISER:
                u = new Cruiser(players[playerId], pos, unitId);
                break;
            case UNIT_TYPE::SUBMARINE:
                u = new Submarine(players[playerId], pos, unitId);
                break;
            case UNIT_TYPE::AIRCRAFT_CARRIER:
                u = new AircraftCarrier(players[playerId], pos, unitId);
                break;
            case UNIT_TYPE::MISSILE_JET:
                u = new MissileJet(players[playerId], pos, unitId, false);
                break;
            case UNIT_TYPE::DEMO_JET:
                u = new DemoJet(players[playerId], pos, unitId, false);
                break;
            default:
                break;
        }

        players[playerId]->addUnit(u);
    }
}
