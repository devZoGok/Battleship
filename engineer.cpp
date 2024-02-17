#include "engineer.h"
#include "activeGameState.h"
#include "structure.h"
#include "player.h"
#include "game.h"
#include "map.h"

#include <solUtil.h>
#include <stateManager.h>

#include <vector>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	Engineer::Engineer(Player *player, int id, Vector3 pos, Quaternion rot, Unit::State state) : Vehicle(player, id, pos, rot, state){
		hackRange = generateView()["units"][id + 1]["hackRange"];

		Vector2 size = Vector2(lenHpBar, 10);
		hackStatusBackground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 0, 1));
		hackStatusForeground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(1, 0, 1, 1));
	}

	Engineer::~Engineer(){
		removeBar(hackStatusBackground);
		removeBar(hackStatusForeground);
	}

	void Engineer::update(){
		Vehicle::update();

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		vector<Player*> selectingPlayers = Unit::getSelectingPlayers();
		bool mainPlayerSelecting = (activeState && find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());

		if(hackStatus < 100)
			Unit::displayUnitStats(hackStatusForeground, hackStatusBackground, hackStatus, 100, mainPlayer == player && mainPlayerSelecting, Vector2(0, -10));
		else{
			hackStatusBackground->setVisible(false);
			hackStatusForeground->setVisible(false);
		}

		if(orders.empty() || orders[0].type != Order::TYPE::HACK)
			hackStatus = 0;
	}

	void Engineer::build(Order order){
		if(pathPoints.empty()){
			if(!order.targets[0].unit)
				player->addUnit(order.targets[0].unit);
			else {
				Structure *structure = (Structure*)order.targets[0].unit;
				sol::table targTable = generateView()["units"][order.targets[0].unit->getId()];
				int costRate = (int)targTable["cost"] / 100, buildRate = (int)targTable["buildTime"] / 100;

				if(structure->getBuildStatus() < 100 && player->getRefineds() >= costRate && getTime() - lastIncrementTime > buildRate){
					structure->incrementBuildStatus();
					player->setRefineds(player->getRefineds() - costRate);
					lastIncrementTime = getTime();
				}
				else if(structure->getBuildStatus() >= 100){
					removeOrder(0);
					player->incStructuresBuilt();
				}
			}
		}
		else{
			navigate(0.5 * Map::getSingleton()->getCellSize().x);

			if(type == UnitType::LAND)
				alignToSurface();
		}
	}

	void Engineer::hack(Order order){
		Unit *targUnit = order.targets[0].unit;
		bool withinRange = (targUnit->getPos().getDistanceFrom(pos) < hackRange);
		int hackRate = int(generateView()["units"][id + 1]["hackTime"]) / 100;
		bool canHack = (getTime() - lastIncrementTime > hackRate);

		if(withinRange && canHack){
			hackStatus++;
			pursuingTarget = false;
		}
		else if(!withinRange){
			hackStatus = 0;

			if(!pursuingTarget){
				preparePathpoints(order, targUnit->getPos());
				pursuingTarget = true;
			}
			else
				navigateToTarget(.9 * hackRange);
		}

		if(hackStatus >= 100){
			Game::getSingleton()->changeUnitPlayer(order.targets[0].unit, player);
			removeOrder(0);
		}
	}
}
