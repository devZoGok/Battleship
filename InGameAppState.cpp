#include"InGameAppState.h"
#include"ConsoleCommand.h"
#include<algorithm>
#include"Vessel.h"
#include"AircraftCarrier.h"

using namespace game::gui;
using namespace game::util;
using namespace game::content;
using namespace irr::video;
using namespace std;

namespace game{
    namespace core{
        InGameAppState::ResumeButton::ResumeButton(GuiAppState *guiState, InGameAppState *inGameState, GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            this->guiState = guiState;
            this->inGameState = inGameState;
        }

        void InGameAppState::ResumeButton::onClick() {
            inGameState->toggleMainMenu();
        }

        GuiAppState* InGameAppState::ResumeButton::getGuiState() {
            return guiState;
        }

        InGameAppState::ConsoleButton::ConsoleButton(GuiAppState *guiState, InGameAppState *inGameState, GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            this->guiState = guiState;
            this->inGameState = inGameState;
        }

        void InGameAppState::ConsoleButton::onClick() {

            class ConsoleCommandEntryButton : public Button {
            public:

                ConsoleCommandEntryButton(GameManager *gM, InGameAppState *inGameState, Textbox *t, Listbox *l, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
                    this->inGameState = inGameState;
                    textbox = t;
                    listbox = l;
                }

                void onClick() {
                    stringw name;
                    vector<stringw> args;
                    vector<int> spaceIds;
                    for (int i = 0; i < textbox->getEntry().size(); i++) {
                        if (textbox->getEntry()[i] == ' ') {
                            spaceIds.push_back(i);
                        }
                    }
                    if (spaceIds.size() > 0) {
                        for (int i = 0; i < spaceIds[0]; i++)
                            name += textbox->getEntry()[i];
                        for (int i = 0; i < spaceIds.size() - 1; i++) {
                            stringw w;
                            for (int i2 = spaceIds[i]; i2 < spaceIds[i + 1]; i2++)
                                w += textbox->getEntry()[i2];
                            args.push_back(w);
                        }
                        args.push_back("");
                        for (int i = spaceIds[spaceIds.size() - 1]; i < textbox->getEntry().size(); i++) {
                            args[args.size() - 1] += textbox->getEntry()[i];
                        }
                    } else
                        name = textbox->getEntry();
                    ConsoleCommand c(gameManager, listbox, inGameState->getPlayerList(), name, args);
                }
            private:
                InGameAppState *inGameState;
                Textbox *textbox;
                Listbox *listbox;
            };
            vector<stringw> list;
            int emptyEntries = 20;
            for (int i = 0; i < emptyEntries; i++)
                list.push_back("");
            vector2d<s32> pos(300, 100);
            Listbox *consoleListbox = new Listbox(gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(420, 20), list, 20);
            consoleListbox->openUp();
            guiState->addListbox(consoleListbox);
            Textbox *consoleTextbox = new Textbox(gameManager, vector2d<s32>(pos.X, pos.Y + 20 * (emptyEntries + 1)), vector2d<s32>(300, 20));
            guiState->addTextbox(consoleTextbox);
            ConsoleCommandEntryButton *entryButton = new ConsoleCommandEntryButton(gameManager, inGameState, consoleTextbox, consoleListbox, vector2d<s32>(pos.X + 320, pos.Y + 20 * (emptyEntries + 1)), vector2d<s32>(100, 20), "Enter", true);
            guiState->addButton(entryButton);
        }

        InGameAppState::InGameOptionsButton::ReturnButton::ReturnButton(GuiAppState *guiState, InGameAppState *inGameState, GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
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
            vector2d<s32> pos(100, 100);
            ResumeButton *resumeButton = new ResumeButton(guiState, inGameState, gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(150, 50), "Resume", true);
            ConsoleButton *consoleButton = new ConsoleButton(guiState, inGameState, gameManager, vector2d<s32>(pos.X, pos.Y + 60), vector2d<s32>(150, 50), "Console", true);
            InGameOptionsButton *optionsButton = new InGameOptionsButton(guiState, inGameState, gameManager, vector2d<s32>(pos.X, pos.Y + 120), vector2d<s32>(150, 50), "Options", true);
            MainMenuButton *mainMenuButton = new MainMenuButton(guiState, inGameState, gameManager, vector2d<s32>(pos.X, pos.Y + 180), vector2d<s32>(150, 50), "Main menu", true);
            ExitButton *exitButton = new ExitButton(gameManager, vector2d<s32>(pos.X, pos.Y + 240), vector2d<s32>(150, 50), "Exit", true);
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

        InGameAppState::InGameOptionsButton::InGameOptionsButton(GuiAppState *guiState, InGameAppState *inGameState, GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : OptionsButton(gM, pos, size, name, separate) {
            this->guiState = guiState;
            this->inGameState = inGameState;
        }

        void InGameAppState::InGameOptionsButton::onClick() {
            returnButton = new ReturnButton(guiState, inGameState, gameManager, vector2d<s32>(50, gameManager->getHeight() - 150), vector2d<s32>(150, 50), "Back", true);
            guiState->addButton(returnButton);
            guiState->removeButton("Resume");
            guiState->removeButton("Console");
            guiState->removeButton("Main menu");
            guiState->removeButton("Exit");
            OptionsButton::onClick();
        }

        InGameAppState::MainMenuButton::MainMenuButton(GuiAppState *guiState, InGameAppState *inGameState, GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            this->guiState = guiState;
            this->inGameState = inGameState;
        }

        void InGameAppState::MainMenuButton::onClick() {

        }

        InGameAppState::UnitCreationButton::UnitCreationButton(GameManager *gM, ITexture *icon, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            setImageButton(new Image(gameManager, icon, pos, size));
        }

        void InGameAppState::UnitCreationButton::onClick() {
        }

        void InGameAppState::UnitCreationButton::update() {
            Button::update();
        }

        InGameAppState::BattleshipCreationButton::BattleshipCreationButton(GameManager *gM, ITexture *icon, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate)
        : InGameAppState::UnitCreationButton(gM, icon, pos, size, name, separate) {
        }

        void InGameAppState::BattleshipCreationButton::onClick() {
        }

        InGameAppState::DestroyerCreationButton::DestroyerCreationButton(GameManager *gM, ITexture *icon, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate)
        : InGameAppState::UnitCreationButton(gM, icon, pos, size, name, separate) {
        }

        void InGameAppState::DestroyerCreationButton::onClick() {
        }

        InGameAppState::CruiserCreationButton::CruiserCreationButton(GameManager *gM, ITexture *icon, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate)
        : InGameAppState::UnitCreationButton(gM, icon, pos, size, name, separate) {
        }

        void InGameAppState::CruiserCreationButton::onClick() {
        }

        InGameAppState::CarrierCreationButton::CarrierCreationButton(GameManager *gM, ITexture *icon, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate)
        : InGameAppState::UnitCreationButton(gM, icon, pos, size, name, separate) {
        }

        void InGameAppState::CarrierCreationButton::onClick() {
        }

        InGameAppState::SubmarineCreationButton::SubmarineCreationButton(GameManager *gM, ITexture *icon, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate)
        : InGameAppState::UnitCreationButton(gM, icon, pos, size, name, separate) {
        }

        void InGameAppState::SubmarineCreationButton::onClick() {
        }

        InGameAppState::InGameAppState(GameManager *gM, vector<stringw> difficultyLevels, vector<stringw> factions) {
            gameManager = gM;
            type = AppStateTypes::IN_GAME_STATE;
            this->playerId = 0;
            this->difficultyLevels = difficultyLevels;
            this->factions = factions;
        }

        InGameAppState::~InGameAppState() {
        }

        void InGameAppState::onAttachment() {
            AbstractAppState::onAttachment();
            map = new Map(gameManager);
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
                Player *p = new Player(gameManager, difficulty, faction);
                players.push_back(p);
                p->setId(players.size() - 1);
            }
            mainPlayer = players[playerId];
            guiState = ((GuiAppState*)gameManager->getAppState(AppStateTypes::GUI_STATE));
            activeState = new ActiveGameState(gameManager, guiState, map, players, playerId);
            gameManager->attachState(activeState);
        }

        void InGameAppState::onDetachment() {

        }

        void InGameAppState::update() {
            if (map)
                map->update();
            if (isMainMenuActive) {
                IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
                driver->draw2DRectangle(SColor(100, 0, 0, 0), rect<s32>(0, 0, gameManager->getWidth(), gameManager->getHeight()));
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
                }
            }
        }

        void InGameAppState::attachGui() {
            int idOffset = 0;
            if (mainPlayer->getFaction() == 1)
                idOffset += 7;
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            /*
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
            if (!isMainMenuActive) {
                isMainMenuActive = true;
                gameManager->dettachState(activeState);
                vector2d<s32> pos(100, 100);
                //detachGui();
                resumeButton = new ResumeButton(guiState, this, gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(150, 50), "Resume", true);
                consoleButton = new ConsoleButton(guiState, this, gameManager, vector2d<s32>(pos.X, pos.Y + 60), vector2d<s32>(150, 50), "Console", true);
                optionsButton = new InGameOptionsButton(guiState, this, gameManager, vector2d<s32>(pos.X, pos.Y + 120), vector2d<s32>(150, 50), "Options", true);
                mainMenuButton = new MainMenuButton(guiState, this, gameManager, vector2d<s32>(pos.X, pos.Y + 180), vector2d<s32>(150, 50), "Main menu", true);
                exitButton = new ExitButton(gameManager, vector2d<s32>(pos.X, pos.Y + 240), vector2d<s32>(150, 50), "Exit", true);
                guiState->addButton(resumeButton);
                guiState->addButton(consoleButton);
                guiState->addButton(optionsButton);
                guiState->addButton(mainMenuButton);
                guiState->addButton(exitButton);
                //gameManager->getDevice()->getSceneManager()->getActiveCamera()->setVisible(false);
            } 
            else {
                isMainMenuActive = false;
                gameManager->attachState(activeState);
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

        void InGameAppState::onAction(Bind bind, bool isPressed) {
            switch(bind){
                case TOGGLE_MAIN_MENU: 
                    if(isPressed)toggleMainMenu();
                    break;
            }
        }

        void InGameAppState::onAnalog(Bind bind, double str) {}
    }
}
