#include "mapEditorButton.h"
#include "guiAppState.h"
#include "inGameAppState.h"
#include "mapEditorAppState.h"
#include "concreteGuiManager.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	void MapEditorButton::onClick(){
		ConcreteGuiManager::getSingleton()->readLuaScreenScript("mapEditorMenu.lua");
		/*
		GameManager *gm = GameManager::getSingleton();
		GuiAppState *guiState = (GuiAppState*)gm->getStateManager()->getAppStateByType((int)AppStateType::GUI_STATE);
		guiState->removeAllButtons(vector<Button*>{this});

		int width = gm->getWidth(), height = gm->getHeight();
		Vector2 size = Vector2(150, 40);

		NewMapButton *newMapButton = new NewMapButton(Vector2(width / 16, height / 12), size);
		LoadMapButton *loadMapButton = new LoadMapButton(Vector2(width / 16, height / 12 * 2), size);
		guiState->addButton(newMapButton);
		guiState->addButton(loadMapButton);
		guiState->removeButton(this);
		*/
	}
}
