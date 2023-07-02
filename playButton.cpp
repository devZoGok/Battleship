#include "playButton.h"
#include "gameManager.h"
#include "inGameAppState.h"
#include "concreteGuiManager.h"

#include <stateManager.h>

#include <glfw3.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	PlayButton::PlayButton(vector<Listbox*> difficulties, vector<Listbox*> factions, Listbox *mapListbox, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", GLFW_KEY_P, separate) {
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
	
			StateManager *stateManager = GameManager::getSingleton()->getStateManager();
			int selectedMap = mapListbox->getSelectedOption();
			string mapName = wstringToString(mapListbox->getContents()[selectedMap]);
	        stateManager->attachAppState(new InGameAppState(difficulties, factions, mapName));

			ConcreteGuiManager::getSingleton()->readLuaScreenScript("inGame.lua");
	    }
}
