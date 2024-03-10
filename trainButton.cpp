#include "trainButton.h"
#include "activeGameState.h"
#include "buildableUnit.h"
#include "factory.h"

#include <stateManager.h>
#include <solUtil.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	TrainButton::TrainButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath, int uid, int slId) :
		UnitButton(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, imagePath, uid),
		slotId(slId) {}

	void TrainButton::onClick(){
		ActiveGameState *activeState = (ActiveGameState*)(GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::ACTIVE_STATE));
		Player *player = activeState->getPlayer();
		vector<Unit*> selUnits = player->getSelectedUnits(), factories = player->getUnitsById(unitId);

		for(Unit *fac : factories)
			if(fac->getBuildableUnit(slotId).buildable && find(selUnits.begin(), selUnits.end(), fac) != selUnits.end())
				((Factory*)fac)->appendToQueue(slotId);
	}
}
