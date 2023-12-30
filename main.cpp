#include "util.h"
#include "gameManager.h"
#include "guiAppState.h"
#include "concreteGuiManager.h"

#include <stateManager.h>

#include <assetManager.h>

#include <string>

using namespace battleship;
using namespace vb01;
using namespace std;

int main(int argc, char **argv) {
    GameManager *gm = GameManager::getSingleton();

	string gamePath = string(argv[0]);
	gamePath = gamePath.substr(0, gamePath.find_last_of("/") + 1) + "../";
	gm->start(gamePath);
    gm->getStateManager()->attachAppState(new GuiAppState());

	AssetManager::getSingleton()->load(gm->getPath() + "Fonts/batang.ttf");

	ConcreteGuiManager::getSingleton()->readLuaScreenScript("mainMenu.lua");

	while(true){
		gm->update();
	}

    return 0;
}
