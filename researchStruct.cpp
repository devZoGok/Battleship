#include "researchStruct.h"
#include "player.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	ResearchStruct::ResearchStruct(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus, Unit::State state) : Structure(player, id, pos, rot, buildStatus, state){
		sol::table unitTable = generateView()["units"][id + 1];
		generationRate = unitTable["generationRate"];
		generationSpeed = unitTable["generationSpeed"];
	}

	void ResearchStruct::update(){
		Structure::update();
		int cost = 1;

		if(player->getRefineds() >= cost && canGenerateResearch()){
			player->addResearch(generationSpeed);
			player->subtractRefineds(cost);
			lastGenTime = getTime();
		}
	}

	void ResearchStruct::researchTechnology(int techId){
	}
}
