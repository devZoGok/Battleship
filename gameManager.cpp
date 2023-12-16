#include <algorithm>

#include <stateManager.h>
#include <inputManager.h>

#include <assetManager.h>
#include <root.h>

#include "gameManager.h"
#include "gameObjectFactory.h"
#include "guiAppState.h"
#include "defConfigs.h"
#include "player.h"
#include "resourceDeposit.h"

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

	void GameManager::registerOrderMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<Order> orderType = SOL_LUA_STATE.new_usertype<Order>("Order", sol::constructors<Order(), Order(Order::TYPE, vector<Order::Target>, Vector3, int)>());
		orderType["type"] = &Order::type;
		orderType["direction"] = &Order::direction;
		orderType["targets"] = &Order::targets;

		sol::usertype<Order::Target> targetType = SOL_LUA_STATE.new_usertype<Order::Target>("Target", sol::constructors<Order::Target(), Order::Target(Unit*, Vector3)>());
		targetType["unit"] = &Order::Target::unit;
		targetType["pos"] = &Order::Target::pos;
	}

	void GameManager::registerGameObjectFactoryMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<GameObjectFactory> gmObjFacType = SOL_LUA_STATE.new_usertype<GameObjectFactory>("GameObjectFactory");
		gmObjFacType["createUnit"] = &GameObjectFactory::createUnit;
		gmObjFacType["createResourceDeposit"] = &GameObjectFactory::createResourceDeposit;
	}

	void GameManager::registerUnitMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<Unit> unitType = SOL_LUA_STATE.new_usertype<Unit>("Unit", sol::constructors<Unit(Player*, int, Vector3, Quaternion)>());
		unitType["getUnitClass"] = &Unit::getUnitClass;
	}

	void GameManager::registerPlayerMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<Player> playerType = SOL_LUA_STATE.new_usertype<Player>("Player", sol::constructors<Player(int, int, int, bool, Vector3)>());
		playerType["addUnit"] = &Player::addUnit;
		playerType["getUnit"] = &Player::getUnit;
		playerType["getNumUnits"] = &Player::getNumberOfUnits;
		playerType["issueOrder"] = &Player::issueOrder;
		playerType["selectUnits"] = &Player::selectUnits;
		playerType["deselectUnits"] = &Player::deselectUnits;
		playerType["getSpawnPoint"] = &Player::getSpawnPoint;
		playerType["getSelectedUnitsByClass"] = &Player::getSelectedUnitsByClass;
	}

	void GameManager::registerVectorMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<Vector3> vec3Type = SOL_LUA_STATE.new_usertype<Vector3>("Vector3", sol::constructors<Vector3(), Vector3(float, float, float)>());
		vec3Type["x"] = &Vector3::x;
		vec3Type["y"] = &Vector3::y;
		vec3Type["z"] = &Vector3::z;
	}

	void GameManager::registerQuaternionMembers(){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::usertype<Quaternion> quatType = SOL_LUA_STATE.new_usertype<Quaternion>("Quaternion", sol::constructors<Quaternion(), Quaternion(float, float, float, float)>());
		quatType["w"] = &Quaternion::w;
		quatType["x"] = &Quaternion::x;
		quatType["y"] = &Quaternion::y;
		quatType["z"] = &Quaternion::z;
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
		registerVectorMembers();
		registerQuaternionMembers();
		registerOrderMembers();
		registerGameObjectFactoryMembers();
		registerUnitMembers();
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
