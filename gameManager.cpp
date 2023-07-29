#include <algorithm>

#include <stateManager.h>
#include <inputManager.h>

#include <assetManager.h>
#include <root.h>

#include <luaManager.h>

#include "gameManager.h"
#include "guiAppState.h"
#include "defConfigs.h"

using namespace std;
using namespace vb01;
using namespace gameBase;

namespace battleship{
	static GameManager *gameManager = nullptr;

	GameManager* GameManager::getSingleton(){
		if(!gameManager)
			gameManager = new GameManager();

		return gameManager;
	}

	void GameManager::start(string gameDir) {
		path = gameDir + "Assets/";

		LuaManager *luaManager = LuaManager::getSingleton();
		luaManager->executeCode("PATH = \"" + path + "\";");

		vector<string> files = configData::scripts;
		files.emplace(files.begin(), "Scripts/main.lua");

		for(string &f : files)
			f = path + f;

		luaManager->buildScript(files);

		string ind1 = "graphics", ind2 = "resolution";
		width = luaManager->getIntFromTable(ind1, vector<Index>{Index(ind2), Index("x")});
		height = luaManager->getIntFromTable(ind1, vector<Index>{Index(ind2), Index("y")});

		Root *root = Root::getSingleton();
		root->start(width, height, path + "../external/vb01/", "Battleship");

		stateManager = new StateManager();
    	inputManager = new InputManager(stateManager, root->getWindow());
	}

    void GameManager::update() {
		Root::getSingleton()->update();
        inputManager->update();
		stateManager->update();
    }
}
