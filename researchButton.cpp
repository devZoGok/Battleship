#include "researchButton.h"
#include "activeGameState.h"
#include "researchStruct.h"

#include <stateManager.h>
#include <solUtil.h>

namespace battleship{
	using namespace vb01;
	using namespace std;

	ResearchButton::ResearchButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath, int uid, int tid) :
	   	UnitButton(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, imagePath, uid), techId(tid){}

	void ResearchButton::onClick(){
		ActiveGameState *activeState = (ActiveGameState*)(GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::ACTIVE_STATE));
		Player *player = activeState->getPlayer();
		vector<Unit*> selUnits = player->getSelectedUnits(), researchStructs = player->getUnitsById(unitId);

		for(Unit *rs : researchStructs)
			if(find(selUnits.begin(), selUnits.end(), rs) != selUnits.end())
				((ResearchStruct*)rs)->appendToQueue(techId);
	}
}
