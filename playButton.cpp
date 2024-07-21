#include "playButton.h"
#include "gameManager.h"
#include "inGameAppState.h"
#include "concreteGuiManager.h"
#include "game.h"

#include <stateManager.h>

#include <glfw3.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	PlayButton::PlayButton(vector<Listbox*> difficulties, vector<Listbox*> factions, Listbox *mapListbox, Vector3 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", GLFW_KEY_P, separate) {
	        difficultiesListboxes = difficulties;
	        factionsListboxes = factions;
			this->mapListbox = mapListbox;
	    }
	
	    void PlayButton::onClick() {
	        std::vector<string> difficulties, factions;
	
	        for(int i = 0; i < difficultiesListboxes.size(); i++)
	            difficulties.push_back(wstringToString(difficultiesListboxes[i]->getContents()[difficultiesListboxes[i]->getSelectedOption()]));
	
	        for(int i = 0; i < factionsListboxes.size(); i++)
	            factions.push_back(to_string(factionsListboxes[i]->getSelectedOption()));

			Game *game = Game::getSingleton();
			game->initTechnologies();
	
			int selectedMap = mapListbox->getSelectedOption();
			string mapName = wstringToString(mapListbox->getContents()[selectedMap]);
			Map *map = Map::getSingleton();
			map->load(mapName);

			int numPlayers = map->getNumSpawnPoints();

			for(int i = 0; i < numPlayers; i++){
				bool cpuPlayer = (i < numPlayers - 1);
				string name = (cpuPlayer ? "CPU player #" + to_string(i) : "Player");
				game->addPlayer(new Player(0, 0, i, Vector3(i, i, i), cpuPlayer, map->getSpawnPoint(i), name));
			}

			map->loadPlayerGameObjects();

			ConcreteGuiManager::getSingleton()->readLuaScreenScript("inGame.lua");

			StateManager *stateManager = GameManager::getSingleton()->getStateManager();
	        stateManager->attachAppState(new InGameAppState(difficulties, factions));
	    }
}
