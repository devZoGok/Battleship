#include "researchStruct.h"
#include "player.h"
#include "game.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	ResearchStruct::ResearchStruct(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus, Unit::State state) : Structure(player, id, pos, rot, buildStatus, state){
		sol::table unitTable = generateView()["units"][id + 1];
		generationRate = unitTable["generationRate"];
		generationSpeed = unitTable["generationSpeed"];
		researchCost = unitTable["researchCost"];
	}

	void ResearchStruct::update(){
		Structure::update();

		if(!isComplete()) return;

		if(researchQueue.empty() && health > .3 * maxHealth && player->getRefineds() >= researchCost && canUpdateResearch()){
			player->addResearch(generationSpeed);
			player->subtractRefineds(researchCost);
			lastUpdateTime = getTime();
		}
		else if(!researchQueue.empty()){
			int techCost = Game::getSingleton()->getTechnology(researchQueue[0]).cost;
			int playerResearch = player->getResearch();

			if(techCost > playerResearch && canUpdateResearch()){
				researchStatus += (int)((float)playerResearch / techCost);
				lastUpdateTime = getTime();
			}
			else if(researchStatus >= 100 || techCost <= playerResearch){
				player->addTechnology(researchQueue[0]);
				researchQueue.erase(researchQueue.begin());
				researchStatus = 0;
			}
		}
	}
}
