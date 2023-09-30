#include <algorithm>

#include <button.h>

#include <util.h>
#include <root.h>
#include <camera.h>
#include <assetManager.h>

#include <stateManager.h>

#include "defConfigs.h"
#include "inGameAppState.h"
#include "console.h"
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
    }

	InGameAppState::ConsoleButton::ConsoleButton::ConsoleCommandEntryButton::ConsoleCommandEntryButton(Textbox *t, Listbox *l, Vector2 pos, Vector2 size, string name) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", 257, true) {
	    textbox = t;
	    listbox = l;
	}
	
	void InGameAppState::ConsoleButton::ConsoleButton::ConsoleCommandEntryButton::onClick() {
		Console::execute(wstringToString(textbox->getText()));
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
        GuiAppState *guiState = ((GuiAppState*)stateManager->getAppStateByType((int)AppStateType::GUI_STATE));
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
						
			guiManager->readLuaScreenScript("gamePaused.lua", activeState->getButtons());
        } 
        else {
            isMainMenuActive = false;
            gm->getStateManager()->attachAppState(activeState);
						
			AssetManager::getSingleton()->load(gm->getPath() + (string)generateView()["modelPrefix"], true);

			guiManager->readLuaScreenScript("inGame.lua", activeState->getButtons());

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
