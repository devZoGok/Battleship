#include <cmath>

#include "util.h"
#include "gameManager.h"
#include "stateManager.h"
#include "guiAppState.h"
#include "inGameAppState.h"

using namespace std;
using namespace game;
using namespace game::core;
using namespace game::gui;
using namespace irr::gui;
using namespace irr::scene;

namespace game{
    namespace util{
        ISceneNode* castRay(ISceneManager *smgr, vector3df start, vector3df end){
            ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();
            line3d<float> ray;
            ray.start = start;
            ray.end = end;
            triangle3df t;
            vector3df collPoint;
            ISceneNode *collNode = collMan->getSceneNodeAndCollisionPointFromRay(ray, collPoint, t, 0, 0);
            return collNode;
        }
        
        vector<stringw> readFile(std::string path,int firstLine,int lastLine) {
            vector<stringw> fileLines;
            ifstream inFile;
            inFile.open(path);
            if (inFile.is_open()) {
                int i=0;
                std::string line;
                while (getline(inFile,line)) {
                    if((lastLine!=0&&firstLine<=i&&i<lastLine)||lastLine==-1)
                        fileLines.push_back(stringw(line.c_str()));
                    i++;
                }
            }
            inFile.close();
            return fileLines;
        }

        void writeFile(std::string path, vector<std::string> fileLines) {
            ofstream outFile;
            outFile.open(path);
            if (outFile.is_open()) 
                for (int i = 0; i < fileLines.size(); i++)
                    outFile << fileLines[i] << "\n";
            outFile.close();
        }

        void makeTitlescreenButtons(GuiAppState *state) {

            class SpButton : public Button {
            public:

                SpButton(GuiAppState *state, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(pos, size, name, separate) {
                    this->state = state;
                }

                void onClick() {
                    vector<stringw> difficulties, factions;

                    class PlayButton : public Button {
                    public:
                        PlayButton(Listbox **difficulties, Listbox **factions, int lengths[2], vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(pos, size, name, separate) {
                            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
                            this->lengths[0]=lengths[0];
                            this->lengths[1]=lengths[1];
                            difficultiesListboxes=difficulties;
                            factionsListboxes=factions;
                        }

                        void onClick() {
														GameManager *gm = GameManager::getSingleton();
                            gm->detachAllBitmapTexts();
                            state->removeButton("Back");
//                             gameManager->dettachState(state);
                            std::vector<stringw> difficulties,factions;

                            for(int i=0;i<lengths[0];i++)
                                difficulties.push_back(difficultiesListboxes[i]->getLine(difficultiesListboxes[i]->getSelectedOption()));

                            for(int i=0;i<lengths[1];i++)
                                factions.push_back(stringw(factionsListboxes[i]->getSelectedOption()));

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

                        ReturnButton(GuiAppState *state, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(pos, size, name, separate) {
                            this->state = state;
                        }

                        void onClick() {
                            state->removeAllCheckboxes();
                            state->removeAllListboxes();
                            state->removeAllSliders();
                            state->removeAllTextboxes();
														GameManager::getSingleton()->detachAllBitmapTexts();
                            makeTitlescreenButtons(state);
                            state->removeButton("Play");
                            state->removeButton("Back");
                        }
                    private:
                        GuiAppState *state;
                    };

										GameManager *gm = GameManager::getSingleton();
                    IGUIFont *font = gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
                    vector2d<s32> pos(gm->getWidth() / 8, gm->getHeight() / 8);
                    difficulties.push_back("Easy");
                    difficulties.push_back("Medium");
                    difficulties.push_back("Hard");
                    factions.push_back("0");
                    factions.push_back("1");
                    gm->attachBitmapText(new BitmapText("Difficulty", vector2d<s32>(pos.X, pos.Y - 20), font));
                    gm->attachBitmapText(new BitmapText("Faction", vector2d<s32>(pos.X + 110, pos.Y - 20), font));
                    gm->attachBitmapText(new BitmapText("Player", vector2d<s32>(pos.X - 40, pos.Y), font));
                    gm->attachBitmapText(new BitmapText("CPU", vector2d<s32>(pos.X - 20, pos.Y + 30), font));
                    Listbox *cpuDifficulty = new Listbox(vector2d<s32>(pos.X, pos.Y + 30), vector2d<s32>(100, 20), difficulties, 3);
                    Listbox *cpuFaction = new Listbox(vector2d<s32>(pos.X + 110, pos.Y + 30), vector2d<s32>(100, 20), factions, 2);
                    Listbox *playerFaction = new Listbox(vector2d<s32>(pos.X + 110, pos.Y), vector2d<s32>(100, 20), factions, 2);
                    Listbox **difficultyListboxes=new Listbox*[1];
                    Listbox **factionListboxes=new Listbox*[2];
                    difficultyListboxes[0]=cpuDifficulty;
                    factionListboxes[0]=playerFaction;
                    factionListboxes[1]=cpuFaction;
                    int lengths[]{1,2};
                    PlayButton *playButton = new PlayButton(difficultyListboxes, factionListboxes, lengths, vector2d<s32>(50, gm->getHeight() - 150), vector2d<s32>(140, 50), "Play", true);
                    ReturnButton *returnButton = new ReturnButton(state, vector2d<s32>(200, gm->getHeight() - 150), vector2d<s32>(140, 50), "Back", true);
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

                MainMenuOptionsButton(GuiAppState *state, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : OptionsButton(pos, size, name, separate) {
                    this->state = state;
                }

                void onClick() {
                    ReturnButton *returnButton = new ReturnButton(state, vector2d<s32>(50, GameManager::getSingleton()->getHeight() - 150), vector2d<s32>(150, 50), "Back", true);
                    state->addButton(returnButton);
                    state->removeButton("Singleplayer");
                    state->removeButton("Exit");
                    OptionsButton::onClick();
                }
            private:
                GuiAppState *state;

                class ReturnButton : public Button {
                public:

                    ReturnButton(GuiAppState *state, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(pos, size, name, separate) {
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
                        SpButton *spButton = new SpButton(state, vector2d<s32>(gm->getWidth() / 16, gm->getHeight() / 12), vector2d<s32>(150, 40), "Singleplayer", true);
                        MainMenuOptionsButton *optionsButton = new MainMenuOptionsButton(state, vector2d<s32>(gm->getWidth() / 16, gm->getHeight() / 12 * 2), vector2d<s32>(150, 40), "Options", true);
                        state->addButton(optionsButton);
                        ExitButton *exitButton = new ExitButton(vector2d<s32>(gm->getWidth() / 16, gm->getHeight() / 12 * 3), vector2d<s32>(150, 40), "Exit", true);
                        state->addButton(spButton);
                        state->addButton(exitButton);
                        state->removeButton("Back");
                    }
                private:
                    GuiAppState *state;
                };
            };

						GameManager *gm = GameManager::getSingleton();
            SpButton *spButton = new SpButton(state, vector2d<s32>(gm->getWidth() / 16, gm->getHeight() / 12), vector2d<s32>(150, 40), "Singleplayer", true);
            MainMenuOptionsButton *optionsButton = new MainMenuOptionsButton(state, vector2d<s32>(gm->getWidth() / 16, gm->getHeight() / 12 * 2), vector2d<s32>(150, 40), "Options", true);
            ExitButton *exitButton = new ExitButton(vector2d<s32>(gm->getWidth() / 16, gm->getHeight() / 12 * 3), vector2d<s32>(150, 40), "Exit", true);
            state->addButton(optionsButton);
            state->addButton(spButton);
            state->addButton(exitButton);
        }

        double getAngleBetween(vector3df v1, vector3df v2) {
            v1.normalize();
            v2.normalize();
            return acos(v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z);
        }
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
    }
}
