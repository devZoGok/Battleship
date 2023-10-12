#include "game.h"
#include "player.h"
#include "projectile.h"

namespace battleship{
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
	}

	void Game::togglePause(){
	}
}
