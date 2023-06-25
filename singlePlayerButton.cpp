#include "singlePlayerButton.h"
#include "gameManager.h"
#include "inGameAppState.h"

#include <stateManager.h>

#include <util.h>
#include <listbox.h>

#include <glfw3.h>

namespace battleship{
	using namespace gameBase;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace std;

	SinglePlayerButton::SinglePlayerButton(Vector2 pos, Vector2 size, string name) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", GLFW_KEY_S, true) {}
	
	void SinglePlayerButton::onMouseOver(){
		setColor(Vector4(.8, .8, .8, 1));
	}
	
	void SinglePlayerButton::onMouseOff(){
		setColor(Vector4(.6, .6, .6, 1));
	}
	
	void SinglePlayerButton::onClick() {
	    vector<string> difficulties, factions;
	
	    class PlayButton : public Button {
	    public:
	        PlayButton(Listbox **difficulties, Listbox **factions, Listbox *mapListbox, int lengths[2], Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", GLFW_KEY_P, separate) {
	            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::GUI_STATE));
	            this->lengths[0] = lengths[0];
	            this->lengths[1] = lengths[1];
	
	            difficultiesListboxes = difficulties;
	            factionsListboxes = factions;
				this->mapListbox = mapListbox;
	        }
	
	        void onClick() {
				GameManager *gm = GameManager::getSingleton();
	            std::vector<string> difficulties, factions;
	
	            for(int i = 0; i < lengths[0]; i++)
	                difficulties.push_back(wstringToString(difficultiesListboxes[i]->getContents()[difficultiesListboxes[i]->getSelectedOption()]));
	
	            for(int i = 0; i < lengths[1]; i++)
	                factions.push_back(to_string(factionsListboxes[i]->getSelectedOption()));
	
	
	            delete[] difficultiesListboxes;
	            delete[] factionsListboxes;
	
				StateManager *stateManager = gm->getStateManager();
				int selectedMap = mapListbox->getSelectedOption();
				string mapName = wstringToString(mapListbox->getContents()[selectedMap]);
	            stateManager->attachAppState(new InGameAppState(difficulties, factions, mapName));
	            state->removeAllListboxes();
	            state->removeButton("Back");
	            state->removeButton("Play");
	        }
	    private:
	        GuiAppState *state;
	        Listbox **difficultiesListboxes, **factionsListboxes, *mapListbox = nullptr;
	        int lengths[2];
	    };
	
	    class ReturnButton : public Button {
	    public:
	
	        ReturnButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", GLFW_KEY_B, separate) {
	            this->state = state;
	        }
	
	        void onClick() {
	            state->removeAllCheckboxes();
	            state->removeAllListboxes();
	            state->removeAllSliders();
	            state->removeAllTextboxes();
	            makeTitlescreenButtons(state);
	            state->removeButton("Play");
	            state->removeButton("Back");
	        }
	    private:
	        GuiAppState *state;
	    };
	
		/*
		GameManager *gm = GameManager::getSingleton();
	    Vector2 pos(gm->getWidth() / 8, gm->getHeight() / 8);
	    difficulties.push_back("Easy");
	    difficulties.push_back("Medium");
	    difficulties.push_back("Hard");
	    factions.push_back("0");
	    factions.push_back("1");
	
	
		string font = gm->getPath() + "Fonts/batang.ttf";
	    Listbox *cpuDifficulty = new Listbox(Vector2(pos.x, pos.y + 30), Vector2(100, 20), difficulties, 3, font);
	    Listbox *cpuFaction = new Listbox(Vector2(pos.x + 110, pos.y + 30), Vector2(100, 20), factions, 2, font);
	    Listbox *playerFaction = new Listbox(Vector2(pos.x + 110, pos.y), Vector2(100, 20), factions, 2, font);
	
		vector<string> folders = readDir(gm->getPath() + "Models/Maps", true);
		int numMinDirs = 3;
		int numShowDirs = folders.size() < numMinDirs ? folders.size() : numMinDirs;
		Listbox *map = new Listbox(Vector2(pos.x + 110, pos.y + 100), Vector2(100, 20), folders, numShowDirs, font);
	    Listbox **difficultyListboxes = new Listbox*[1];
	    Listbox **factionListboxes = new Listbox*[2];
						
	    difficultyListboxes[0] = cpuDifficulty;
	    factionListboxes[0] = playerFaction;
	    factionListboxes[1] = cpuFaction;
	
	    int lengths[]{1, 2};
	    PlayButton *playButton = new PlayButton(difficultyListboxes, factionListboxes, map, lengths, Vector2(50, gm->getHeight() - 150), Vector2(140, 50), "Play", true);
	    ReturnButton *returnButton = new ReturnButton(state, Vector2(200, gm->getHeight() - 150), Vector2(140, 50), "Back", true);
	    state->addButton(playButton);
	    state->addButton(returnButton);
	    state->addListbox(cpuDifficulty);
	    state->addListbox(cpuFaction);
	    state->addListbox(playerFaction);
		state->addListbox(map);
	    state->removeButton("Options");
	    state->removeButton("Exit");
	    state->removeButton("Singleplayer");
		*/
	
	}
}
