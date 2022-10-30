#include <algorithm>

#include <stateManager.h>
#include <inputManager.h>

#include <assetManager.h>
#include <root.h>

#include <luaManager.h>

#include "gameManager.h"
#include "guiAppState.h"

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

    GameManager::GameManager() {
		LuaManager *luaManager = LuaManager::getSingleton();
		string pathBase = "../Assets/Scripts/";
		luaManager->buildScript(vector<string>{pathBase + "defPaths.lua"});
		path = luaManager->getString("PATH");

		luaManager->buildScript(vector<string>{pathBase + "options.lua"});
		string ind1 = "graphics", ind2 = "resolution";
		width = luaManager->getIntFromTable(ind1, vector<Index>{Index(ind2), Index("x")});
		height = luaManager->getIntFromTable(ind1, vector<Index>{Index(ind2), Index("y")});
    }

    GameManager::~GameManager() {}

	void GameManager::start() {
		Root *root = Root::getSingleton();
		root->start(width, height, "../../vb01/", "Battleship");

		stateManager = new StateManager();
    	inputManager = new InputManager(stateManager, root->getWindow());
	}

    void GameManager::update() {
		Root::getSingleton()->update();
        inputManager->update();
		stateManager->update();
    }
}
