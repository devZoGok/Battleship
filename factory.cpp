#include "factory.h"
#include "player.h"
#include "gameObjectFactory.h"
#include "activeGameState.h"

#include <stateManager.h>

#include <vector>

namespace battleship{
	using namespace vb01;
	using namespace std;

	Factory::Factory(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus) : Structure(player, id, pos, rot, buildStatus){
		initProperties();
	}

	void Factory::initProperties(){
	}

	void Factory::update(){
		Structure::update();

		if(!unitQueue.empty())
			train();
	}

	void Factory::appendToQueue(int uc){
		unitQueue.push_back(uc);
	}

	void Factory::train(){
		bool training = !unitQueue.empty();
		buildStatusForeground->setVisible(training);
		buildStatusBackground->setVisible(training);

		if(training){
			ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
			Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

			vector<Player*> selectingPlayers = getSelectingPlayers();
			bool mainPlayerSelecting = (activeState && find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());

			Unit::displayUnitStats(buildStatusForeground, buildStatusBackground, trainingStatus, 100, mainPlayer == player && mainPlayerSelecting, Vector2(0, -10));

			if(canTrain()){
				trainingStatus++;
				lastTrainTime = getTime();
			}

			if(trainingStatus >= 100){
				Unit *unit = GameObjectFactory::createUnit(player, unitQueue[0], pos, rot);
				player->addUnit(unit);
				unit->setOrder(Order(Order::TYPE::MOVE, vector<Order::Target>{Order::Target(nullptr, pos + 30 * dirVec)}, Vector3::VEC_ZERO));

				unitQueue.erase(unitQueue.begin());
				trainingStatus = 0;

				player->incVehiclesBuilt();
			}
		}
	}
}
