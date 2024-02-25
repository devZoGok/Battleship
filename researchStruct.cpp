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
		researchCost = unitTable["researchCost"];
	}

	void ResearchStruct::update(){
		Structure::update();

		if(player->getRefineds() >= researchCost && canGenerateResearch()){
			player->addResearch(generationSpeed);
			player->subtractRefineds(researchCost);
			lastGenTime = getTime();
		}
	}

	void ResearchStruct::researchTechnology(int techId){
	}
}
