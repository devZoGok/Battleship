#include <algorithm>

#include <stateManager.h>
#include <inputManager.h>

#include <assetManager.h>
#include <root.h>

#include "gameManager.h"
#include "guiAppState.h"
#include "defConfigs.h"
#include "player.h"

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

	void GameManager::registerPlayerMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<Player> playerType = SOL_LUA_STATE.new_usertype<Player>("Player", sol::constructors<Player(int, int, int, bool, Vector3)>());
	}

	void GameManager::initLua(string gameDir){
		sol::state_view SOL_LUA_STATE = generateView();

		path = gameDir + "Assets/";
		SOL_LUA_STATE.script("PATH = \"" + path + "\";");

		for(string f : configData::scripts)
			SOL_LUA_STATE.script_file(path + f);

		sol::table resTable = SOL_LUA_STATE["graphics"]["resolution"]; 
		width = resTable["x"];
		height = resTable["y"];
	}

	void GameManager::start(string gameDir) {
		registerPlayerMembers();
		initLua(gameDir);

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
