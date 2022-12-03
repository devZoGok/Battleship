#include <algorithm>

#include <button.h>

#include <util.h>
#include <assetManager.h>

#include <stateManager.h>
#include <luaManager.h>

#include "defConfigs.h"
#include "inGameAppState.h"
#include "consoleCommand.h"
#include "vessel.h"

using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
	using namespace configData;

    InGameAppState::ResumeButton::ResumeButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size) : Button(pos, size, "Resume", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::ResumeButton::onClick() {
        inGameState->toggleMainMenu();
    }

    GuiAppState* InGameAppState::ResumeButton::getGuiState() {
        return guiState;
    }

    InGameAppState::ConsoleButton::ConsoleButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size) : Button(pos, size, "Console", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::ConsoleButton::onClick() {
        vector<string> list;
        int emptyEntries = 20;

        for (int i = 0; i < emptyEntries; i++)
            list.push_back("");

        Vector2 pos = Vector2(300, 100);
        Listbox *consoleListbox = new Listbox(
				Vector2(pos.x, pos.y),
			   	Vector2(420, 20),
			   	list,
			   	20,
			   	GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"
				);
        consoleListbox->openUp();
        guiState->addListbox(consoleListbox);

        Textbox *consoleTextbox = new Textbox(
				Vector2(pos.x, pos.y + 20 * (emptyEntries + 1)),
			   	Vector2(300, 20),
			   	GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"
				);
        guiState->addTextbox(consoleTextbox);
		
        entryButton = new ConsoleCommandEntryButton(
				inGameState,
			   	consoleTextbox,
			   	consoleListbox,
			   	Vector2(pos.x + 320, pos.y + 20 * (emptyEntries + 1)),
			   	Vector2(100, 20),
			   	"Enter",
			   	true
				);
        guiState->addButton(entryButton);
    }

		InGameAppState::ConsoleButton::ConsoleButton::ConsoleCommandEntryButton::ConsoleCommandEntryButton(InGameAppState *inGameState, Textbox *t, Listbox *l, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
	        this->inGameState = inGameState;
	        textbox = t;
	        listbox = l;
	    }
	
		void InGameAppState::ConsoleButton::ConsoleButton::ConsoleCommandEntryButton::onClick() {
			ConsoleCommand::execute(wstringToString(textbox->getText()));
	    }

    InGameAppState::InGameOptionsButton::ReturnButton::ReturnButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

	//TODO: replace string literals removeButton() lines
    void InGameAppState::InGameOptionsButton::ReturnButton::onClick() {
        guiState->removeAllCheckboxes();
        guiState->removeAllListboxes();
        guiState->removeAllSliders();
        guiState->removeAllTextboxes();
        guiState->removeButton("Controls");
        guiState->removeButton("Mouse");
        guiState->removeButton("Video");
        guiState->removeButton("Audio");
        guiState->removeButton("Multiplayer");

        Vector2 pos = Vector2(100, 100), size = Vector2(150, 50);
        ResumeButton *resumeButton = new ResumeButton(guiState, inGameState, Vector2(pos.x, pos.y), size);
        ConsoleButton *consoleButton = new ConsoleButton(guiState, inGameState, Vector2(pos.x, pos.y + 60), size);
        InGameOptionsButton *optionsButton = new InGameOptionsButton(guiState, inGameState, Vector2(pos.x, pos.y + 120), size);
        MainMenuButton *mainMenuButton = new MainMenuButton(guiState, inGameState, Vector2(pos.x, pos.y + 180), size);
        ExitButton *exitButton = new ExitButton(Vector2(pos.x, pos.y + 240), size);

        inGameState->setResumeButton(resumeButton);
        inGameState->setConsoleButton(consoleButton);
        inGameState->setOptionsButton(optionsButton);
        inGameState->setMainMenuButton(mainMenuButton);
        inGameState->setExitButton(exitButton);

        guiState->addButton(resumeButton);
        guiState->addButton(consoleButton);
        guiState->addButton(optionsButton);
        guiState->addButton(mainMenuButton);
        guiState->addButton(exitButton);

        guiState->removeButton("Back");
    }

    InGameAppState::InGameOptionsButton::InGameOptionsButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size) : OptionsButton(pos, size, "Options", true) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::InGameOptionsButton::onClick() {
        returnButton = new ReturnButton(guiState, inGameState, Vector2(50, GameManager::getSingleton()->getHeight() - 150), Vector2(150, 50), "Back", true);
        guiState->addButton(returnButton);
        guiState->removeButton(inGameState->getResumeButton());
        guiState->removeButton(inGameState->getConsoleButton());
        guiState->removeButton(inGameState->getMainMenuButton());
        guiState->removeButton(inGameState->getExitButton());
        OptionsButton::onClick();
    }

    InGameAppState::MainMenuButton::MainMenuButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size) : Button(pos, size, "Main menu", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::MainMenuButton::onClick() {

    }

    InGameAppState::InGameAppState(vector<string> difficultyLevels, vector<string> factions, string mapName) : AbstractAppState(
						AppStateType::IN_GAME_STATE,
					 	configData::calcSumBinds(AppStateType::IN_GAME_STATE, true),
					 	configData::calcSumBinds(AppStateType::IN_GAME_STATE, false),
					 	GameManager::getSingleton()->getPath() + "Scripts/options.lua"){
        this->playerId = 0;
        this->difficultyLevels = difficultyLevels;
        this->factions = factions;
		this->mapName = mapName;
    }

    InGameAppState::~InGameAppState() {
    }

    void InGameAppState::onAttached() {
        AbstractAppState::onAttached();

		Map::getSingleton()->load(mapName);

        for (int i = 0; i < factions.size(); i++) {
            int faction, difficulty;

            if (i == 0)
                difficulty = -1;
            else {
                if (difficultyLevels[i - 1] == "Easy")
                    difficulty = 0;
                else if (difficultyLevels[i - 1] == "Medium")
                    difficulty = 1;
                else
                    difficulty = 2;
            }

            faction = factions[i][0] - 48;
            Player *p = new Player(difficulty, faction);
            players.push_back(p);
            p->setId(players.size() - 1);
        }

        mainPlayer = players[playerId];

		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
        guiState = ((GuiAppState*)stateManager->getAppStateByType((int)AppStateType::GUI_STATE));
        activeState = new ActiveGameState(guiState, players, playerId);
        stateManager->attachAppState(activeState);

		AssetManager *assetManager = AssetManager::getSingleton();
		assetManager->load(DEFAULT_TEXTURE);

		loadModels();
    }

    void InGameAppState::onDettached() {}

	void InGameAppState::updateUnits(Player *p){
		for (int i = 0; i < p->getNumberOfUnits(); i++){
		    Unit *u = p->getUnit(i);
		
		    if(u->isWorking())
		        u->update();
		    else{
		        int selectedId = -1;
		        std::vector<Unit*> &units = p->getUnits(), &selectedUnits = activeState->getSelectedUnits();
		        units.erase(units.begin() + i);
		
		        for(int j = 0; j < maxNumGroups; j++){
		            int id = -1;
		            std::vector<Unit*> &group = activeState->getUnitGroup(j);
		
		            for(int k = 0; k < group.size() && id == -1; k++)
		                if(group[k] == u)
		                    id = k;
		
		            if(id != -1)
		                group.erase(group.begin() + id);
		        }
		
		        for(int j = 0; j < selectedUnits.size() && selectedId == -1; j++)
		            if(selectedUnits[j] == u)
		                selectedId = j;
		
		        if(selectedId != -1)
		            selectedUnits.erase(selectedUnits.begin() + selectedId);
		
		        delete u;
		    }
		}
	}

	void InGameAppState::updateProjectiles(){
        for(int i = 0; i < projectiles.size(); i++){
            Projectile *p = projectiles[i];

            if(!p->isExploded())
                p->update();
            else{
                delete p;
                projectiles.erase(projectiles.begin() + i);
            }
        }
	}

    void InGameAppState::update() {
        for (Player *p : players)
            if (p) {
                p->update();
				updateUnits(p);
            }

		updateProjectiles();
    }

	void InGameAppState::loadModels(){
		LuaManager *lm = LuaManager::getSingleton();
		string pathBase = GameManager::getSingleton()->getPath() + "Scripts/";
		lm->buildScript(configData::scripts);
		int numUnits = lm->getInt("numUnits");
		modelPaths.clear();

		for(int i = 0; i < numUnits; ++i){
			string basePath = lm->getStringFromTable("basePath", vector<Index>{Index(i + 1)});
			string meshPath = lm->getStringFromTable("meshPath", vector<Index>{Index(i + 1)});
			string modelPath = basePath + meshPath;
			AssetManager::getSingleton()->load(modelPath);
			modelPaths.push_back(modelPath);
		}
	}

    void InGameAppState::toggleMainMenu() {
		GameManager *gm = GameManager::getSingleton();

        if (!isMainMenuActive) {
            isMainMenuActive = true;
            gm->getStateManager()->dettachAppState(activeState);
						
            Vector2 pos = Vector2(100, 100);
            resumeButton = new ResumeButton(guiState, this, Vector2(pos.x, pos.y), Vector2(150, 50));
            consoleButton = new ConsoleButton(guiState, this, Vector2(pos.x, pos.y + 60), Vector2(150, 50));
            optionsButton = new InGameOptionsButton(guiState, this, Vector2(pos.x, pos.y + 120), Vector2(150, 50));
            mainMenuButton = new MainMenuButton(guiState, this, Vector2(pos.x, pos.y + 180), Vector2(150, 50));
            exitButton = new ExitButton(Vector2(pos.x, pos.y + 240), Vector2(150, 50));
            guiState->addButton(resumeButton);
            guiState->addButton(consoleButton);
            guiState->addButton(optionsButton);
            guiState->addButton(mainMenuButton);
            guiState->addButton(exitButton);
        } 
        else {
            isMainMenuActive = false;
            gm->getStateManager()->attachAppState(activeState);
						
            guiState->removeAllCheckboxes();
            guiState->removeAllListboxes();
            guiState->removeAllSliders();
            guiState->removeAllTextboxes();
            guiState->removeButton(consoleButton->getEntryButton());
            guiState->removeButton(consoleButton);
            guiState->removeButton(mainMenuButton);
            guiState->removeButton(optionsButton);
            guiState->removeButton(exitButton);
            guiState->removeButton(resumeButton);

			loadModels();

			for(Player *p : players)
				for(Unit *u : p->getUnits())
					u->reinitUnit();
        }
    }

    void InGameAppState::onAction(int bind, bool isPressed) {
        switch((Bind)bind){
			case Bind::TOGGLE_MAIN_MENU: 
            	if(isPressed && !activeState->isPlacingStructures())
					toggleMainMenu();
            break;
        }
    }

    void InGameAppState::onAnalog(int bind, float str) {}
}
