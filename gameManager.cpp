#include <algorithm>

#include <stateManager.h>
#include <inputManager.h>

#include <assetManager.h>
#include <root.h>

#include "gameManager.h"
#include "gameObjectFactory.h"
#include "game.h"
#include "guiAppState.h"
#include "defConfigs.h"
#include "player.h"
#include "factory.h"
#include "engineer.h"
#include "pointDefense.h"
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

	void GameManager::registerMembers(){
		sol::state_view SOL_LUA_STATE = generateView();

		SOL_LUA_STATE.new_usertype<Order>(
			"Order", sol::constructors<Order(), Order(Order::TYPE, vector<Order::Target>, Vector3, int)>(),
			"type", &Order::type,
			"direction", &Order::direction,
			"targets", &Order::targets
		);

		SOL_LUA_STATE.new_usertype<Order::Target>(
			"Target", sol::constructors<Order::Target(), Order::Target(Unit*, Vector3)>(),
			"unit", &Order::Target::unit,
			"pos", &Order::Target::pos
		);

		SOL_LUA_STATE.new_usertype<GameObjectFactory>(
			"GameObjectFactory",
			"createUnit", &GameObjectFactory::createUnit,
			"createResourceDeposit", &GameObjectFactory::createResourceDeposit
		);

		SOL_LUA_STATE.new_usertype<Unit>(
			"Unit", sol::constructors<Unit(Player*, int, Vector3, Quaternion)>(),
			"setState", &Unit::setState,
			"getOrder", &Unit::getOrder,
			"getNumOrders", &Unit::getNumOrders,
			"getPos", &GameObject::getPos,
			"getUnitClass", &Unit::getUnitClass,
			"toEngineer", &Unit::toEngineer,
			"toPointDefense", &Unit::toPointDefense,
			"toStructure", &Unit::toStructure,
			"toFactory", &Unit::toFactory
		);

		SOL_LUA_STATE.new_usertype<Structure>(
			"Structure", sol::constructors<Structure(Player*, int, Vector3, Quaternion, int)>(),
			"getPos", &GameObject::getPos,
			"getBuildStatus", &Structure::getBuildStatus
		);

		SOL_LUA_STATE.new_usertype<Factory>(
			"Factory", sol::constructors<Factory(Player*, int, Vector3, Quaternion, int)>(),
			"getPos", &GameObject::getPos,
			"appendToQueue", &Factory::appendToQueue,
			"getBuildStatus", &Structure::getBuildStatus,
			"getNumQueueUnitsById", &Factory::getNumQueueUnitsById,
			"getQueue", &Factory::getQueue
		);

		SOL_LUA_STATE.new_usertype<Player>(
			"Player", sol::constructors<Player(int, int, int, Vector3, bool, Vector3, string)>(),
			"getSelectedUnits", &Player::getSelectedUnits,
			"addUnit", &Player::addUnit,
			"getUnit", &Player::getUnit,
			"getNumUnits", &Player::getNumUnits,
			"getResourceDeposits", &Player::getResourceDeposits,
			"issueOrder", &Player::issueOrder,
			"selectUnits", &Player::selectUnits,
			"deselectUnits", &Player::deselectUnits,
			"getSpawnPoint", &Player::getSpawnPoint,
			"getUnitsById", &Player::getUnitsById,
			"getUnitsByClass", &Player::getUnitsByClass
		);

		SOL_LUA_STATE.new_usertype<ResourceDeposit>(
			"ResourceDeposit", sol::constructors<ResourceDeposit(Player*, int, Vector3, Quaternion, int)>(),
			"getPos", &GameObject::getPos
		);

		SOL_LUA_STATE.new_usertype<Game>(
			"Game",
			"getSingleton", &Game::getSingleton,
			"getPlayers", &Game::getPlayers
		);

		SOL_LUA_STATE.new_usertype<Vector3>(
			"Vector3", sol::constructors<Vector3(), Vector3(float, float, float)>(),
			"x", &Vector3::x,
			"y", &Vector3::y,
			"z", &Vector3::z,
			"norm", &Vector3::norm,
			"getDistanceFrom", &Vector3::getDistanceFrom,
			"add", [](Vector3 v1, Vector3 v2){return v1 + v2;},
			"subtr", [](Vector3 v1, Vector3 v2){return v1 - v2;},
			"mult", [](Vector3 v1, float s){return v1 * s;},
			"div", [](Vector3 v1, float s){return v1 / s;}
		);

		SOL_LUA_STATE.new_usertype<Quaternion>(
			"Quaternion", sol::constructors<Quaternion(), Quaternion(float, float, float, float), Quaternion(float, Vector3)>(),
			"w", &Quaternion::w,
			"x", &Quaternion::x,
			"y", &Quaternion::y,
			"z", &Quaternion::z,
			"multVec", [](Quaternion q, Vector3 v){return q * v;}
		);
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
		registerMembers();
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
