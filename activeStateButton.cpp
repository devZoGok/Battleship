#include "activeStateButton.h"
#include "gameManager.h"
#include "activeGameState.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	ActiveStateButton::ActiveStateButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, true, imagePath){
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		ActiveGameState *activeState = (ActiveGameState*)stateManager->getAppStateByType((int)AppStateType::ACTIVE_STATE);

		if(activeState)
			activeState->addButton(this);
	}

	void ActiveStateButton::onClick(){
	}
}
