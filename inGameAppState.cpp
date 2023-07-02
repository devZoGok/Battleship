#include <algorithm>

#include <button.h>

#include <util.h>
#include <root.h>
#include <camera.h>
#include <assetManager.h>

#include <stateManager.h>
#include <luaManager.h>

#include "defConfigs.h"
#include "inGameAppState.h"
#include "consoleCommand.h"
#include "unitFrameController.h"
#include "concreteGuiManager.h"
#include "vessel.h"

using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
	using namespace configData;

    InGameAppState::ResumeButton::ResumeButton(Vector2 pos, Vector2 size) : Button(pos, size, "Resume", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) {}

    void InGameAppState::ResumeButton::onClick() {
		StateManager *sm = GameManager::getSingleton()->getStateManager();
        ((InGameAppState*)sm->getAppStateByType((int)AppStateType::IN_GAME_STATE))->toggleMainMenu();
    }

    InGameAppState::ConsoleButton::ConsoleButton(Vector2 pos, Vector2 size) : Button(pos, size, "Console", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) { }

    void InGameAppState::ConsoleButton::onClick() {
		ConcreteGuiManager::getSingleton()->readLuaScreenScript("console.lua");
		/*
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
		 */
    }

		InGameAppState::ConsoleButton::ConsoleButton::ConsoleCommandEntryButton::ConsoleCommandEntryButton(Textbox *t, Listbox *l, Vector2 pos, Vector2 size, string name) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) {
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
		/*
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
		 */
    }

    InGameAppState::InGameOptionsButton::InGameOptionsButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size) : OptionsButton(pos, size, "Options", true) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::InGameOptionsButton::onClick() {
		/*
        returnButton = new ReturnButton(guiState, inGameState, Vector2(50, GameManager::getSingleton()->getHeight() - 150), Vector2(150, 50), "Back", true);
        guiState->addButton(returnButton);
        guiState->removeButton(inGameState->getResumeButton());
        guiState->removeButton(inGameState->getConsoleButton());
        guiState->removeButton(inGameState->getMainMenuButton());
        guiState->removeButton(inGameState->getExitButton());
        OptionsButton::onClick();
		 */
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
        this->playerId = 1;
        this->difficultyLevels = difficultyLevels;
        this->factions = factions;
		this->mapName = mapName;
    }

    InGameAppState::~InGameAppState() {
    }

    void InGameAppState::onAttached() {
        AbstractAppState::onAttached();

		Map *map = Map::getSingleton();
		map->load(mapName);

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
            Player *p = new Player(difficulty, faction, i);
            p->setId(map->getNumPlayers() - 1);
            map->addPlayer(p);
        }

		Camera *cam = Root::getSingleton()->getCamera();
		cam->setPosition(map->getSpawnPoint(playerId - 1) + Vector3(1, 1, 1) * 40);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());

        mainPlayer = map->getPlayer(playerId);

		GameManager *gm = GameManager::getSingleton();
		StateManager *stateManager = gm->getStateManager();
        guiState = ((GuiAppState*)stateManager->getAppStateByType((int)AppStateType::GUI_STATE));
        activeState = new ActiveGameState(guiState, playerId);
        stateManager->attachAppState(activeState);
    }

    void InGameAppState::onDettached() {}

    void InGameAppState::update() {
		Map::getSingleton()->update();
    }

    void InGameAppState::toggleMainMenu() {
		GameManager *gm = GameManager::getSingleton();
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();

        if (!isMainMenuActive) {
            isMainMenuActive = true;
            gm->getStateManager()->dettachAppState(activeState);
						
			guiManager->readLuaScreenScript("gamePaused.lua");
        } 
        else {
            isMainMenuActive = false;
            gm->getStateManager()->attachAppState(activeState);
						
			AssetManager::getSingleton()->load(gm->getPath() + LuaManager::getSingleton()->getString("modelPrefix"), true);

			guiManager->readLuaScreenScript("inGame.lua");

			for(Player *p : Map::getSingleton()->getPlayers())
				for(Unit *u : p->getUnits())
					u->reinitUnit();
        }
    }

    void InGameAppState::onAction(int bind, bool isPressed) {
        switch((Bind)bind){
			case Bind::TOGGLE_MAIN_MENU: 
            	if(isPressed && !UnitFrameController::getSingleton()->isPlacingFrames())
					toggleMainMenu();
            break;
        }
    }

    void InGameAppState::onAnalog(int bind, float str) {}
}
