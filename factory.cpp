#include "factory.h"
#include "player.h"
#include "gameObjectFactory.h"

namespace battleship{
	using namespace vb01;

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
			Unit::displayUnitStats(buildStatusForeground, buildStatusBackground, trainingStatus, 100, Vector2(0, -10));

			if(canTrain()){
				trainingStatus++;
				lastTrainTime = getTime();
			}

			if(trainingStatus >= 100){
				unitQueue.erase(unitQueue.begin());
				trainingStatus = 0;
				player->addUnit(GameObjectFactory::createUnit(player, unitQueue[0], pos + 30 * dirVec, rot));
			}
		}
	}
}
