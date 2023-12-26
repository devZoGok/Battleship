#include "statsButton.h"
#include "gameManager.h"
#include "concreteGuiManager.h"
#include "inGameAppState.h"
#include "map.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	StatsButton::StatsButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, true, imagePath){}

	void StatsButton::onClick(){
		Map::getSingleton()->unload();

		StateManager *sm = GameManager::getSingleton()->getStateManager();
	   	InGameAppState *inGameState = (InGameAppState*)sm->getAppStateByType(int(AppStateType::IN_GAME_STATE));
		sm->dettachAppState(inGameState);
		delete inGameState;

		ConcreteGuiManager::getSingleton()->readLuaScreenScript("statistics.lua");
	}
}
