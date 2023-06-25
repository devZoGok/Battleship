#include "util.h"
#include "gameManager.h"
#include "guiAppState.h"
#include "concreteGuiManager.h"

#include <stateManager.h>

#include <assetManager.h>

using namespace battleship;
using namespace vb01;

int main() {
    GameManager *gm = GameManager::getSingleton();
	gm->start();
    gm->getStateManager()->attachAppState(new GuiAppState());

	AssetManager::getSingleton()->load(gm->getPath() + "Fonts/batang.ttf");

	ConcreteGuiManager::getSingleton()->readLuaScreenScript("mainMenu.lua");

	while(true){
		gm->update();
	}

    return 0;
}
