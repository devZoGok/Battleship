#include "unitButton.h"
#include "gameManager.h"
#include "activeGameState.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	UnitButton::UnitButton(Vector2 pos, Vector2 size, string name, string fontPath, int trigger, string imagePath) : Button(pos, size, name, fontPath, trigger, true, imagePath){
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		ActiveGameState *activeState = (ActiveGameState*)stateManager->getAppStateByType((int)AppStateType::ACTIVE_STATE);

		if(activeState)
			activeState->addButton(this);
	}
}
