#include "game.h"
#include "player.h"
#include "projectile.h"

#include <SFML/Audio.hpp>

namespace battleship{
	using namespace vb01;

	static Game *game = nullptr;

	Game* Game::getSingleton(){
		if(!game)
			game = new Game();

		return game;
	}

	void Game::update(){
		for(Player *p : players)
			p->update();

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
	}
}
