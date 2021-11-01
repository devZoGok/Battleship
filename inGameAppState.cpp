#include <algorithm>
#include <button.h>

#include "stateManager.h"
#include "inGameAppState.h"
#include "consoleCommand.h"
#include "aircraftCarrier.h"
#include "vessel.h"

using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
		using namespace configData;

    InGameAppState::ResumeButton::ResumeButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::ResumeButton::onClick() {
        inGameState->toggleMainMenu();
    }

    GuiAppState* InGameAppState::ResumeButton::getGuiState() {
        return guiState;
    }

    InGameAppState::ConsoleButton::ConsoleButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::ConsoleButton::onClick() {

        class ConsoleCommandEntryButton : public Button {
        public:

            ConsoleCommandEntryButton(InGameAppState *inGameState, Textbox *t, Listbox *l, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
                this->inGameState = inGameState;
                textbox = t;
                listbox = l;
            }

            void onClick() {
                wstring name;
                vector<string> args;
                vector<int> spaceIds;

                for (int i = 0; i < textbox->getText().size(); i++) {
                    if (textbox->getText()[i] == ' ') {
                        spaceIds.push_back(i);
                    }
                }

                if (spaceIds.size() > 0) {
                    for (int i = 0; i < spaceIds[0]; i++)
                        name += textbox->getText()[i];

                    for (int i = 0; i < spaceIds.size() - 1; i++) {
                        string w;
                        for (int i2 = spaceIds[i]; i2 < spaceIds[i + 1]; i2++)
                            w += textbox->getText()[i2];
                        args.push_back(w);
                    }

                    args.push_back("");

                    for (int i = spaceIds[spaceIds.size() - 1]; i < textbox->getText().size(); i++) {
                        args[args.size() - 1] += textbox->getText()[i];
                    }
                } else
                    name = textbox->getText();

                //ConsoleCommand c(listbox, inGameState->getPlayerList(), name, args);
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
        Listbox *consoleListbox = new Listbox(Vector2(pos.x, pos.y), Vector2(420, 20), list, 20, PATH + "Fonts/batang.ttf");
        consoleListbox->openUp();
        guiState->addListbox(consoleListbox);
        Textbox *consoleTextbox = new Textbox(Vector2(pos.x, pos.y + 20 * (emptyEntries + 1)), Vector2(300, 20), PATH + "Fonts/batang.ttf");
        guiState->addTextbox(consoleTextbox);
        ConsoleCommandEntryButton *entryButton = new ConsoleCommandEntryButton(inGameState, consoleTextbox, consoleListbox, Vector2(pos.x + 320, pos.y + 20 * (emptyEntries + 1)), Vector2(100, 20), "Enter", true);
        guiState->addButton(entryButton);
    }

    InGameAppState::InGameOptionsButton::ReturnButton::ReturnButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
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

    InGameAppState::MainMenuButton::MainMenuButton(GuiAppState *guiState, InGameAppState *inGameState, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
        this->guiState = guiState;
        this->inGameState = inGameState;
    }

    void InGameAppState::MainMenuButton::onClick() {

    }

    InGameAppState::UnitCreationButton::UnitCreationButton(string icon, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
        //setImageButton(new Image(icon, pos, size));
    }

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

    InGameAppState::InGameAppState(vector<string> difficultyLevels, vector<string> factions) {
        type = AppStateTypes::IN_GAME_STATE;
        this->playerId = 0;
        this->difficultyLevels = difficultyLevels;
        this->factions = factions;
    }

    InGameAppState::~InGameAppState() {
    }

    void InGameAppState::onAttachment() {
        AbstractAppState::onAttachment();
        map = new Map();
        map->load();

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
        guiState = ((GuiAppState*)stateManager->getAppState(AppStateTypes::GUI_STATE));
        activeState = new ActiveGameState(guiState, map, players, playerId);
        stateManager->attachState(activeState);
    }

    void InGameAppState::onDetachment() {

    }

    void InGameAppState::update() {
        if (map)
            map->update();

        if (isMainMenuActive) {
						GameManager *gm = GameManager::getSingleton();
						/*
            IVideoDriver *driver = gm->getDevice()->getVideoDriver();
            driver->draw2DRectangle(SColor(100, 0, 0, 0), rect<s32>(0, 0, gm->getWidth(), gm->getHeight()));
						*/
        }

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

                        if(u->getType()==unitData::UNIT_TYPE::MISSILE_JET||u->getType()==unitData::UNIT_TYPE::DEMO_JET){
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
                        else if(u->getType()==unitData::UNIT_TYPE::AIRCRAFT_CARRIER){
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

        for(int i=0;i<fx.size();i++){
            if(getTime()-fx[i].initTime>fx[i].time){
								/*
                IParticleSystemSceneNode *node=fx[i].node;

                if(node){
                    node->removeAllAffectors();
                    node->setVisible(false);
                }

                if(fx[i].sfx){
                    delete fx[i].sfx->getBuffer();
                    delete fx[i].sfx;
                }

                fx.erase(fx.begin()+i);
								*/
            }
        }
    }

    void InGameAppState::attachGui() {
        int idOffset = 0;

        if (mainPlayer->getFaction() == 1)
            idOffset += 7;

        /*
        IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
        bcb = new BattleshipCreationButton(gameManager, driver->getTexture(iconPath[idOffset]), vector2d<s32>(gameManager->getWidth() - 100, gameManager->getHeight() - 540), vector2d<s32>(100, 100), "battleships", true);
        dcb = new DestroyerCreationButton(gameManager, driver->getTexture(iconPath[idOffset + 1]), vector2d<s32>(gameManager->getWidth() - 100, gameManager->getHeight() - 430), vector2d<s32>(100, 100), "destroyers", true);
        crcb = new CruiserCreationButton(gameManager, driver->getTexture(iconPath[idOffset + 2]), vector2d<s32>(gameManager->getWidth() - 100, gameManager->getHeight() - 320), vector2d<s32>(100, 100), "cruisers", true);
        ccb = new CarrierCreationButton(gameManager, driver->getTexture(iconPath[idOffset + 3]), vector2d<s32>(gameManager->getWidth() - 100, gameManager->getHeight() - 210), vector2d<s32>(100, 100), "carriers", true);
        scb = new SubmarineCreationButton(gameManager, driver->getTexture(iconPath[idOffset + 4]), vector2d<s32>(gameManager->getWidth() - 100, gameManager->getHeight() - 100), vector2d<s32>(100, 100), "submarines", true);
        */
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
            gm->getStateManager()->dettachState(activeState);
            Vector2 pos = Vector2(100, 100);
            //detachGui();
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
            //gameManager->getDevice()->getSceneManager()->getActiveCamera()->setVisible(false);
        } 
        else {
            isMainMenuActive = false;
            gm->getStateManager()->attachState(activeState);
            //attachGui();
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
            //gameManager->getDevice()->getSceneManager()->getActiveCamera()->setVisible(true);
        }
    }

    void InGameAppState::onAction(Mapping::Bind bind, bool isPressed) {
        switch(bind){
						case Mapping::TOGGLE_MAIN_MENU: 
                if(isPressed)toggleMainMenu();
                break;
        }
    }

    void InGameAppState::onAnalog(Mapping::Bind bind, double str) {}
}
