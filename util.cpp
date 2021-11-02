#include <cmath>
#include <vector.h>

#include "util.h"
#include "gameManager.h"
#include "stateManager.h"
#include "guiAppState.h"
#include "inGameAppState.h"

using namespace std;
using namespace vb01;
using namespace vb01Gui;

namespace battleship{
		using namespace configData;

    void makeTitlescreenButtons(GuiAppState *state) {

        class SpButton : public Button {
        public:
            SpButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, separate) {
                this->state = state;
            }

						void onMouseOver(){
								setColor(Vector4(.8, .8, .8, 1));
						}

						void onMouseOff(){
								setColor(Vector4(.6, .6, .6, 1));
						}

            void onClick() {
                vector<string> difficulties, factions;

                class PlayButton : public Button {
                public:
                    PlayButton(Listbox **difficulties, Listbox **factions, int lengths[2], Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, separate) {
                        this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
                        this->lengths[0]=lengths[0];
                        this->lengths[1]=lengths[1];

                        difficultiesListboxes=difficulties;
                        factionsListboxes=factions;
                    }

                    void onClick() {
												GameManager *gm = GameManager::getSingleton();
                        //gm->detachAllBitmapTexts();
                        state->removeButton("Back");
                        gm->getStateManager()->dettachState(state);
                        std::vector<string> difficulties, factions;

												/*
                        for(int i=0;i<lengths[0];i++)
                            difficulties.push_back(difficultiesListboxes[i]->getContents()[difficultiesListboxes[i]->getSelectedOption()]);

                        for(int i=0;i<lengths[1];i++)
                            factions.push_back(factionsListboxes[i]->getSelectedOption());
														*/

                        gm->getStateManager()->attachState(new InGameAppState(difficulties, factions));
                        state->removeAllListboxes();
                        delete[] difficultiesListboxes;
                        delete[] factionsListboxes;
                        state->removeButton("Play");
                    }
                private:
                    GuiAppState *state;
                    Listbox **difficultiesListboxes, **factionsListboxes;
                    int lengths[2];
                };

                class ReturnButton : public Button {
                public:

                    ReturnButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, separate) {
                        this->state = state;
                    }

                    void onClick() {
                        state->removeAllCheckboxes();
                        state->removeAllListboxes();
                        state->removeAllSliders();
                        state->removeAllTextboxes();
												//GameManager::getSingleton()->detachAllBitmapTexts();
                        makeTitlescreenButtons(state);
                        state->removeButton("Play");
                        state->removeButton("Back");
                    }
                private:
                    GuiAppState *state;
                };

								GameManager *gm = GameManager::getSingleton();
                //IGUIFont *font = gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
                Vector2 pos(gm->getWidth() / 8, gm->getHeight() / 8);
                difficulties.push_back("Easy");
                difficulties.push_back("Medium");
                difficulties.push_back("Hard");
                factions.push_back("0");
                factions.push_back("1");
								/*
                gm->attachBitmapText(new BitmapText("Difficulty", vector2d<s32>(pos.X, pos.Y - 20), font));
                gm->attachBitmapText(new BitmapText("Faction", vector2d<s32>(pos.X + 110, pos.Y - 20), font));
                gm->attachBitmapText(new BitmapText("Player", vector2d<s32>(pos.X - 40, pos.Y), font));
                gm->attachBitmapText(new BitmapText("CPU", vector2d<s32>(pos.X - 20, pos.Y + 30), font));
								*/
								string font = PATH + "Fonts/batang.ttf";
                Listbox *cpuDifficulty = new Listbox(Vector2(pos.x, pos.y + 30), Vector2(100, 20), difficulties, 3, font);
                Listbox *cpuFaction = new Listbox(Vector2(pos.x + 110, pos.y + 30), Vector2(100, 20), factions, 2, font);
                Listbox *playerFaction = new Listbox(Vector2(pos.x + 110, pos.y), Vector2(100, 20), factions, 2, font);
                Listbox **difficultyListboxes=new Listbox*[1];
                Listbox **factionListboxes=new Listbox*[2];
                difficultyListboxes[0]=cpuDifficulty;
                factionListboxes[0]=playerFaction;
                factionListboxes[1]=cpuFaction;
                int lengths[]{1,2};
                PlayButton *playButton = new PlayButton(difficultyListboxes, factionListboxes, lengths, Vector2(50, gm->getHeight() - 150), Vector2(140, 50), "Play", true);
                ReturnButton *returnButton = new ReturnButton(state, Vector2(200, gm->getHeight() - 150), Vector2(140, 50), "Back", true);
                state->addButton(playButton);
                state->addButton(returnButton);
                state->addListbox(cpuDifficulty);
                state->addListbox(cpuFaction);
                state->addListbox(playerFaction);
                state->removeButton("Options");
                state->removeButton("Exit");
                state->removeButton("Singleplayer");
            }
        private:
            GuiAppState *state;
        };

        class MainMenuOptionsButton : public OptionsButton {
        public:

            MainMenuOptionsButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : OptionsButton(pos, size, name, separate) {
                this->state = state;
            }

            void onClick() {
                ReturnButton *returnButton = new ReturnButton(state, Vector2(50, GameManager::getSingleton()->getHeight() - 150), Vector2(150, 50), "Back", true);
                state->addButton(returnButton);
                state->removeButton("Singleplayer");
                state->removeButton("Exit");
                OptionsButton::onClick();
            }
        private:
            GuiAppState *state;

            class ReturnButton : public Button {
            public:

                ReturnButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, separate) {
                    this->state = state;
                }

                void onClick() {
                    state->removeAllCheckboxes();
                    state->removeAllListboxes();
                    state->removeAllSliders();
                    state->removeAllTextboxes();
                    state->removeButton("Controls");
                    state->removeButton("Mouse");
                    state->removeButton("Video");
                    state->removeButton("Audio");
                    state->removeButton("Multiplayer");
										GameManager *gm = GameManager::getSingleton();
                    SpButton *spButton = new SpButton(state, Vector2(gm->getWidth() / 16, gm->getHeight() / 12), Vector2(150, 40), "Singleplayer", true);
                    MainMenuOptionsButton *optionsButton = new MainMenuOptionsButton(state, Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 2), Vector2(150, 40), "Options", true);
                    state->addButton(optionsButton);
                    ExitButton *exitButton = new ExitButton(Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 3), Vector2(150, 40), "Exit", true);
                    state->addButton(spButton);
                    state->addButton(exitButton);
                    state->removeButton("Back");
                }
            private:
                GuiAppState *state;
            };
        };

				GameManager *gm = GameManager::getSingleton();
				string font = PATH + "Fonts/batang.ttf";
        SpButton *spButton = new SpButton(state, Vector2(gm->getWidth() / 16, gm->getHeight() / 12), Vector2(150, 40), "Singleplayer", true);
        MainMenuOptionsButton *optionsButton = new MainMenuOptionsButton(state, Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 2), Vector2(150, 40), "Options", true);
        ExitButton *exitButton = new ExitButton(Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 3), Vector2(150, 40), "Exit", true);
        state->addButton(optionsButton);
        state->addButton(spButton);
        state->addButton(exitButton);
    }

		/*
    bool isWithinRect(vector3df c1,vector3df c2, vector3df p, vector3df dir){
        float mainAngle=getAngleBetween(c1-c2,dir);
        float mainHyp=(c1-c2).getLength();
        float mainWidth=cos(mainAngle)*mainHyp,mainLength=sin(mainAngle)*mainHyp;
        vector3df center=(c2-c1)/2+c1;
        float angle=getAngleBetween(center-p,dir);
        float hyp=(center-p).getLength();
        float width=cos(angle)*hyp,length=sin(angle)*hyp;
        return width<=mainWidth/2&&length<=mainLength/2?true:false;
    }
    bool isWithinCuboid(vector3df c0, vector3df c1,vector3df c2,vector3df c3, vector3df p0){
        vector3df aVec=c1-c0,bVec=c2-c0,cVec=c3-c0,pVec=p0-c0;
        float a=aVec.getLength(),b=bVec.getLength(),c=cVec.getLength(),p=pVec.getLength();
        float aAngle=getAngleBetween(pVec,aVec);
        float bAngle=getAngleBetween(pVec,bVec);
        float cAngle=getAngleBetween(pVec,cVec);
        bool withinA=aAngle<=PI/2&&p*cos(aAngle)<=a;
        bool withinB=bAngle<=PI/2&&p*cos(bAngle)<=b;
        bool withinC=cAngle<=PI/2&&p*cos(cAngle)<=c;
        return withinA&&withinB&&withinC;
    }
		*/
}
