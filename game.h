#ifndef GAME_H
#define GAME_H

#include "fx.h"
#include "technology.h"
#include "ability.h"

#include <vector>

namespace sf{
	class Sound;
}

namespace battleship{
	class Unit;
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
			void changeUnitPlayer(Unit*, Player*);
			void initTechnologies();
			float calcAbilFromTech(Ability::Type, std::vector<int>, int, int);
			bool isUnitUnlocked(std::vector<int>, int);
			inline void addFx(Fx f){fx.push_back(f);}
			inline void addPlayer(Player *pl){players.push_back(pl);}
			inline std::vector<Player*>& getPlayers(){return players;}
			inline Player* getPlayer(int id){return players[id];}
			inline int getNumPlayers(){return players.size();}
			inline Technology getTechnology(int id){return technologies[id];}
			inline Ability getAbility(int id){return abilities[id];}
		private:
			Game(){}
			void resetLuaGameObjects();
			void endGame(bool);
			std::vector<int> parseTechTable(int, std::string, std::string, std::string);

			bool paused = false, ended = false;
			std::vector<Fx> fx;
			std::vector<Technology> technologies;
			std::vector<Ability> abilities;
			std::vector<Player*> players;
	};
}

#endif
