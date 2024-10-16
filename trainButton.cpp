#include "trainButton.h"
#include "activeGameState.h"
#include "factory.h"

#include <stateManager.h>
#include <solUtil.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	TrainButton::TrainButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath, int fid, int tuid) : 
		factoryId(fid), 
		trainableUnitId(tuid), 
		UnitButton(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, imagePath){}

	void TrainButton::onClick(){
		ActiveGameState *activeState = (ActiveGameState*)(GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::ACTIVE_STATE));
		Player *player = activeState->getPlayer();
		UnitClass facClass = (UnitClass)generateView()["units"]["unitClass"][factoryId + 1]; 
		vector<Unit*> factories = player->getSelectedUnitsByClass(facClass);

		for(Unit *fac : factories)
			((Factory*)fac)->appendToQueue(trainableUnitId);
	}
}
