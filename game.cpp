#include "game.h"
#include "player.h"
#include "projectile.h"
#include "gameManager.h"
#include "activeGameState.h"
#include "inGameAppState.h"
#include "concreteGuiManager.h"
#include "defConfigs.h"

#include <assetManager.h>

#include <stateManager.h>

#include <solUtil.h>

#include <SFML/Audio.hpp>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	static Game *game = nullptr;

	Game* Game::getSingleton(){
		if(!game)
			game = new Game();

		return game;
	}

	void Game::resetLuaGameObjects(){
		sol::state_view SOL_LUA_VIEW = generateView();
		SOL_LUA_VIEW.script("game.players = {}");

		for(int i = 0; i < players.size(); i++)
			SOL_LUA_VIEW.script("game.players[" + to_string(i + 1) + "] = Player:new()");
	}

	void Game::update(){
		resetLuaGameObjects();

		for(int i = 0; i < players.size(); i++){
			players[i]->update();

			if(players[i]->isCpuPlayer())
				generateView().script("game.players[" + to_string(i + 1) + "]:update()");
		}

		for(Projectile *proj : projectiles)
			proj->update();

		for(int i = 0; i < fx.size(); i++){
			if(getTime() - fx[i].initTime > fx[i].vfxTime)
				removeFx(i, true);

			if(getTime() - fx[i].initTime > fx[i].sfxTime)
				removeFx(i, false);
		}
	}

	void Game::removeFx(int id, bool vfx){
		const sf::SoundBuffer *buffer = fx[id].sfx->getBuffer();

		if(!vfx){
			fx[id].sfx->stop();
			delete fx[id].sfx;
			delete buffer;
			fx.erase(fx.begin() + id);
		}

		if(fx[id].peNode && vfx){
			Root::getSingleton()->getRootNode()->dettachChild(fx[id].peNode);
			delete fx[id].peNode;
			fx[id].peNode = nullptr;
		}
	}

	void Game::togglePause(){
		GameManager *gm = GameManager::getSingleton();
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		ActiveGameState *activeState = ((InGameAppState*)gm->getStateManager()->getAppStateByType(AppStateType::IN_GAME_STATE))->getActiveState();

        if (!paused) {
            gm->getStateManager()->dettachAppState(activeState);
			guiManager->readLuaScreenScript("gamePaused.lua", activeState->getButtons());
        } 
        else {
			for(string f : configData::scripts)
				generateView().script_file(gm->getPath() + f);

			guiManager->readLuaScreenScript("inGame.lua", activeState->getButtons());
			AssetManager::getSingleton()->load(gm->getPath() + (string)generateView()["modelPrefix"], true);

			for(Player *p : Game::getSingleton()->getPlayers())
				for(Unit *u : p->getUnits())
					u->reinit();

            gm->getStateManager()->attachAppState(activeState);
        }

		paused = !paused;
	}
}
