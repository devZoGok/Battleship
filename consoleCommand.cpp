#include <sstream>
#include <string>
#include <cmath>
#include <vector.h>

#include "consoleCommand.h"
#include "unitData.h"
#include "aircraftCarrier.h"
#include "cruiser.h"
#include "destroyer.h"
#include "submarine.h"
#include "demoJet.h"
#include "missileJet.h"

using namespace std;
using namespace vb01;
using namespace vb01Gui;

namespace battleship{
		using namespace unitData;

    ConsoleCommand::ConsoleCommand(Listbox *l, vector<Player*> players, string name, vector<string> args) {
        listbox = l;
        this->players = players;
        this->name = name;
        arguments = args;
        execute();
    }

    ConsoleCommand::~ConsoleCommand() {
    }

    void ConsoleCommand::execute() {
        if (name == commandList[0])
            addUnit();
        else if (name == commandList[1])
            debugUnits();
        else
            printConsoleMessage("No such command");
    }

    void ConsoleCommand::addUnit() {
        int unitId = 0, playerId = 0;
        Vector3 pos(0, 0, 0);
        float angle = 0;

        for (int i = 0; i < arguments.size(); i++) {
            arguments[i].erase(0);
            for (int i2 = 0; i2 < arguments[i].size(); i2++) {
                if (i == 0)
                    playerId += (arguments[i][i2] - 48) * pow(10, arguments[i].size() - (i2 + 1));
                else
                    unitId += (arguments[i][i2] - 48) * pow(10, arguments[i].size() - (i2 + 1));
            }
        }

        if ((playerId == 0 || playerId == 1) && (unitId >= 0 && unitId <= numberOfUnits)) {
            Unit *u = nullptr;

            switch (unitType[unitId]) {
                case UNIT_TYPE::VESSEL:
                    u = new Vessel(players[playerId],pos, unitId);
                    break;
                case UNIT_TYPE::DESTROYER:
                    u = new Destroyer(players[playerId], pos, unitId);
                    break;
                case UNIT_TYPE::CRUISER:
                    u = new Cruiser(players[playerId],pos, unitId);
                    break;
                case UNIT_TYPE::SUBMARINE:
                    u = new Submarine(players[playerId], pos, unitId);
                    break;
                case UNIT_TYPE::AIRCRAFT_CARRIER:
                {
                    u = new AircraftCarrier(players[playerId],pos, unitId);
                    break;
                }
                case UNIT_TYPE::MISSILE_JET:
                    u = new MissileJet(players[playerId],pos, unitId, false);
                    break;
                case UNIT_TYPE::DEMO_JET:
                    u = new DemoJet(players[playerId],pos, unitId, false);
                    break;
                default:
                    break;
            }

            players[playerId]->addUnit(u);
        } else if (playerId != 0 && playerId != 1)
            printConsoleMessage("Invalid player id");
        else if (unitId < 0 || unitId > numberOfUnits)
            printConsoleMessage("Invalid unit id");
    }

    void ConsoleCommand::debugUnits() {
        if (arguments.size() == 0)
            for (Player *pl : players){
                vector<Unit*> &units=pl->getUnits();

                for (Unit *u : units){
                    u->toggleDebugging(true);
                    vector<Projectile*> projectiles=u->getProjectiles();

                    for(Projectile *pr : projectiles)
                        pr->debug();
                }
            }
        else
            printConsoleMessage("Too many arguments");
    }

    void ConsoleCommand::printConsoleMessage(string message) {
        int messageId = 0;
        bool foundEmptySlot = false;

        for (int i = 0; i < listbox->getContents().size() && !foundEmptySlot; i++)
            if (listbox->getContents()[i] == L"") {
                messageId = i;
                foundEmptySlot = true;
            }
				/*
        if (foundEmptySlot)
            listbox->changeLine(messageId, message);
        else
            listbox->addLine(message);
						*/
    }
}
