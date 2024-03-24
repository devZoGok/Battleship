#include "unitButton.h"
#include "gameManager.h"
#include "activeGameState.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	UnitButton::UnitButton(Vector2 pos, Vector2 size, string name, string fontPath, int trigger, string imagePath, int uid) : Button(pos, size, name, fontPath, trigger, true, imagePath), unitId(uid){
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		ActiveGameState *activeState = (ActiveGameState*)stateManager->getAppStateByType((int)AppStateType::ACTIVE_STATE);

		if(activeState)
			activeState->addButton(this);
	}

	//TODO optimize this code
	vector<Unit*> UnitButton::getUnits(int unitId){
		vector<Unit*> units;

		ActiveGameState *activeState = (ActiveGameState*)(GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::ACTIVE_STATE));
		Player *player = activeState->getPlayer();
		vector<Unit*> selUnits = player->getSelectedUnits(), researchStructs = player->getUnitsById(unitId);

		for(Unit *rs : researchStructs)
			if(find(selUnits.begin(), selUnits.end(), rs) != selUnits.end())
				units.push_back(rs);

		return units;
	}
}
