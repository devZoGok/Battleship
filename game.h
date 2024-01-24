#ifndef GAME_H
#define GAME_H

#include "fx.h"

#include <vector>

namespace sf{
	class Sound;
}

namespace battleship{
	class Player;
	class Projectile;

	class Game{
		public:
			static Game* getSingleton();
			void update();
			void togglePause();
			void removeFx(int, bool);
			void removeAllElements();
			void explode(vb01::Vector3, int, float, sf::Sound*);
			inline void addFx(Fx f){fx.push_back(f);}
			inline void addPlayer(Player *pl){players.push_back(pl);}
			inline std::vector<Player*>& getPlayers(){return players;}
			inline Player* getPlayer(int id){return players[id];}
			inline int getNumPlayers(){return players.size();}
		private:
			Game(){}
			void resetLuaGameObjects();
			void endGame(bool);

			bool paused = false, ended = false;
			std::vector<Fx> fx;
			std::vector<Player*> players;
	};
}

#endif
