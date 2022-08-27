#include <algorithm>
#include <button.h>

#include <util.h>
#include <assetManager.h>

#include <stateManager.h>

#include "inGameAppState.h"
#include "unitDataManager.h"
#include "consoleCommand.h"
#include "aircraftCarrier.h"
#include "vessel.h"

using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
		using namespace configData;

    InGameAppState::ResumeButton::ResumeButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::ResumeButton::onClick() {
        inGameState->toggleMainMenu();
    }

    GuiAppState* InGameAppState::ResumeButton::getGuiState() {
        return guiState;
    }

    InGameAppState::ConsoleButton::ConsoleButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::ConsoleButton::onClick() {

        class ConsoleCommandEntryButton : public Button {
        public:

            ConsoleCommandEntryButton(InGameAppState *inGameState, Textbox *t, Listbox *l, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
                this->inGameState = inGameState;
                textbox = t;
                listbox = l;
            }

            void onClick() {
								ConsoleCommand::execute(wstringToString(textbox->getText()));
            }
        private:
            InGameAppState *inGameState;
            Textbox *textbox;
            Listbox *listbox;
        };

        vector<string> list;
        int emptyEntries = 20;

        for (int i = 0; i < emptyEntries; i++)
            list.push_back("");

        Vector2 pos = Vector2(300, 100);
        Listbox *consoleListbox = new Listbox(Vector2(pos.x, pos.y), Vector2(420, 20), list, 20, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf");
        consoleListbox->openUp();
        guiState->addListbox(consoleListbox);
        Textbox *consoleTextbox = new Textbox(Vector2(pos.x, pos.y + 20 * (emptyEntries + 1)), Vector2(300, 20), GameManager::getSingleton()->getPath() + "Fonts/batang.ttf");
        guiState->addTextbox(consoleTextbox);
        ConsoleCommandEntryButton *entryButton = new ConsoleCommandEntryButton(inGameState, consoleTextbox, consoleListbox, Vector2(pos.x + 320, pos.y + 20 * (emptyEntries + 1)), Vector2(100, 20), "Enter", true);
        guiState->addButton(entryButton);
    }

    InGameAppState::InGameOptionsButton::ReturnButton::ReturnButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

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
        Vector2 pos = Vector2(100, 100);
        ResumeButton *resumeButton = new ResumeButton(guiState, inGameState, Vector2(pos.x, pos.y), Vector2(150, 50), "Resume", true);
        ConsoleButton *consoleButton = new ConsoleButton(guiState, inGameState, Vector2(pos.x, pos.y + 60), Vector2(150, 50), "Console", true);
        InGameOptionsButton *optionsButton = new InGameOptionsButton(guiState, inGameState, Vector2(pos.x, pos.y + 120), Vector2(150, 50), "Options", true);
        MainMenuButton *mainMenuButton = new MainMenuButton(guiState, inGameState, Vector2(pos.x, pos.y + 180), Vector2(150, 50), "Main menu", true);
        ExitButton *exitButton = new ExitButton(Vector2(pos.x, pos.y + 240), Vector2(150, 50), "Exit", true);
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

    InGameAppState::InGameOptionsButton::InGameOptionsButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : OptionsButton(pos, size, name, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::InGameOptionsButton::onClick() {
        returnButton = new ReturnButton(guiState, inGameState, Vector2(50, GameManager::getSingleton()->getHeight() - 150), Vector2(150, 50), "Back", true);
        guiState->addButton(returnButton);
        guiState->removeButton("Resume");
        guiState->removeButton("Console");
        guiState->removeButton("Main menu");
        guiState->removeButton("Exit");
        OptionsButton::onClick();
    }

    InGameAppState::MainMenuButton::MainMenuButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::MainMenuButton::onClick() {

    }

    InGameAppState::UnitCreationButton::UnitCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {}

    void InGameAppState::UnitCreationButton::onClick() {
    }

    void InGameAppState::UnitCreationButton::update() {
        Button::update();
    }

    InGameAppState::BattleshipCreationButton::BattleshipCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate)
    : InGameAppState::UnitCreationButton(icon, pos, size, name, separate) {
    }

    void InGameAppState::BattleshipCreationButton::onClick() {
    }

    InGameAppState::DestroyerCreationButton::DestroyerCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate)
    : InGameAppState::UnitCreationButton(icon, pos, size, name, separate) {
    }

    void InGameAppState::DestroyerCreationButton::onClick() {
    }

    InGameAppState::CruiserCreationButton::CruiserCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate)
    : InGameAppState::UnitCreationButton(icon, pos, size, name, separate) {
    }

    void InGameAppState::CruiserCreationButton::onClick() {
    }

    InGameAppState::CarrierCreationButton::CarrierCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate)
    : InGameAppState::UnitCreationButton(icon, pos, size, name, separate) {
    }

    void InGameAppState::CarrierCreationButton::onClick() {
    }

    InGameAppState::SubmarineCreationButton::SubmarineCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate)
    : InGameAppState::UnitCreationButton(icon, pos, size, name, separate) {
    }

    void InGameAppState::SubmarineCreationButton::onClick() {
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
            int faction;
            int difficulty;

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

		UnitDataManager *unitDataManager = UnitDataManager::getSingleton();
		int numUnits = unitDataManager->getNumUnits();
		string *basePaths = unitDataManager->getBasePath();
		string *meshPaths = unitDataManager->getMeshPath();

		for(int i = 0; i < numUnits; ++i)
			assetManager->load(basePaths[i] + meshPaths[i]);
    }

    void InGameAppState::onDettached() {

    }

    void InGameAppState::update() {
        for (Player *p : players)
            if (p) {
                p->update();

                for (int i=0;i<p->getNumberOfUnits();i++){
                    Unit *u=p->getUnit(i);

                    if(u->isWorking())
                        u->update();
                    else{
                        int selectedId=-1;
                        std::vector<Unit*> &units=p->getUnits(),&selectedUnits=activeState->getSelectedUnits();
                        units.erase(units.begin()+i);

                        if(u->getType() == UnitType::MISSILE_JET || u->getType() == UnitType::DEMO_JET){
                            AircraftCarrier *carrier=nullptr;

                            for(int i2=0;i2<p->getUnits().size()&&!carrier;i2++){
                                AircraftCarrier *a=((Jet*)u)->getAircraftCarrier();

                                if(a)
                                    carrier=a;
                            }

                            if(carrier){
                                Jet** jets=carrier->getJets();
                                jets[((Jet*)u)->getJetId()]=nullptr;
                            }
                        }
                        else if(u->getType() == UnitType::AIRCRAFT_CARRIER){
                            AircraftCarrier *carrier=(AircraftCarrier*)u;

                            for(int i2=0;i2<carrier->getMaxNumJets();i2++){
                                Jet *j=carrier->getJet(i2);

                                if(j&&j->isOnBoard())
                                    j->blowUp();
                            }
                        }
                                    
                        for(int i2=0;i2<10;i2++){
                            int id=-1;
                            std::vector<Unit*> &group=activeState->getUnitGroup(i2);

                            for(int i3=0;i3<group.size()&&id==-1;i3++)
                                if(group[i3]==u)
                                    id=i3;

                            if(id!=-1)
                                group.erase(group.begin()+id);
                        }

                        for(int i2=0;i2<selectedUnits.size()&&selectedId==-1;i2++)
                            if(selectedUnits[i2]==u)
                                selectedId=i2;

                        if(selectedId!=-1)
                            selectedUnits.erase(selectedUnits.begin()+selectedId);

                        delete u;
                    }
                }
            }

        for(int i=0;i<projectiles.size();i++){
            Projectile *p=projectiles[i];

            if(!p->isExploded()){
                p->update();
            }
            else{
                delete p;
                projectiles.erase(projectiles.begin()+i);
            }
        }
    }

    void InGameAppState::attachGui() {
        int idOffset = 0;

        if (mainPlayer->getFaction() == 1)
            idOffset += 7;

        guiState->addButton(bcb);
        guiState->addButton(dcb);
        guiState->addButton(crcb);
        guiState->addButton(ccb);
        guiState->addButton(scb);
    }

    void InGameAppState::detachGui() {
        guiState->removeButton(bcb);
        guiState->removeButton(dcb);
        guiState->removeButton(crcb);
        guiState->removeButton(ccb);
        guiState->removeButton(scb);
    }

    void InGameAppState::toggleMainMenu() {
				GameManager *gm = GameManager::getSingleton();

        if (!isMainMenuActive) {
            isMainMenuActive = true;
            gm->getStateManager()->dettachAppState(activeState);
						
            Vector2 pos = Vector2(100, 100);
            resumeButton = new ResumeButton(guiState, this, Vector2(pos.x, pos.y), Vector2(150, 50), "Resume", true);
            consoleButton = new ConsoleButton(guiState, this, Vector2(pos.x, pos.y + 60), Vector2(150, 50), "Console", true);
            optionsButton = new InGameOptionsButton(guiState, this, Vector2(pos.x, pos.y + 120), Vector2(150, 50), "Options", true);
            mainMenuButton = new MainMenuButton(guiState, this, Vector2(pos.x, pos.y + 180), Vector2(150, 50), "Main menu", true);
            exitButton = new ExitButton(Vector2(pos.x, pos.y + 240), Vector2(150, 50), "Exit", true);
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
            guiState->removeButton("Enter");
            guiState->removeButton("Console");
            guiState->removeButton("Main menu");
            guiState->removeButton("Options");
            guiState->removeButton("Exit");
            guiState->removeButton("Resume");
        }
    }

    void InGameAppState::onAction(int bind, bool isPressed) {
        switch((Bind)bind){
						case Bind::TOGGLE_MAIN_MENU: 
                if(isPressed)toggleMainMenu();
                break;
        }
    }

    void InGameAppState::onAnalog(int bind, float str) {}
}
