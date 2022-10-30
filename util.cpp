#include <cmath>

#include <camera.h>
#include <root.h>
#include <vector.h>
#include <util.h>
#include <assetManager.h>

#include <stateManager.h>

#include <glm.hpp>
#include <ext.hpp>

#include <tinydir.h>

#include "util.h"
#include "defConfigs.h"
#include "gameManager.h"
#include "guiAppState.h"
#include "inGameAppState.h"

using namespace std;
using namespace glm;
using namespace vb01;
using namespace vb01Gui;

namespace battleship{
		using namespace configData;

    void makeTitlescreenButtons(GuiAppState *state) {

        class SpButton : public Button {
        public:
            SpButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", GLFW_KEY_S, separate) {
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

								GameManager *gm = GameManager::getSingleton();
                Vector2 pos(gm->getWidth() / 8, gm->getHeight() / 8);
                difficulties.push_back("Easy");
                difficulties.push_back("Medium");
                difficulties.push_back("Hard");
                factions.push_back("0");
                factions.push_back("1");

				tinydir_dir dir;
				tinydir_open_sorted(&dir, (gm->getPath() + "Models/Maps").c_str());
				vector<string> folders;
				
				for (int i = 0; i < dir.n_files; i++) {
					tinydir_file file;
					tinydir_readfile_n(&dir, &file, i);

					if (file.is_dir && file.name[0] != '.')
						folders.push_back(file.name);
				}
				
				tinydir_close(&dir);

								string font = gm->getPath() + "Fonts/batang.ttf";
                Listbox *cpuDifficulty = new Listbox(Vector2(pos.x, pos.y + 30), Vector2(100, 20), difficulties, 3, font);
                Listbox *cpuFaction = new Listbox(Vector2(pos.x + 110, pos.y + 30), Vector2(100, 20), factions, 2, font);
                Listbox *playerFaction = new Listbox(Vector2(pos.x + 110, pos.y), Vector2(100, 20), factions, 2, font);
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

						void onMouseOver(){
								setColor(Vector4(.8, .8, .8, 1));
						}

						void onMouseOff(){
								setColor(Vector4(.6, .6, .6, 1));
						}
        private:
            GuiAppState *state;

            class ReturnButton : public Button {
            public:

                ReturnButton(GuiAppState *state, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, separate) {
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
                    ExitButton *exitButton = new ExitButton(Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 3), Vector2(150, 40));
                    state->addButton(spButton);
                    state->addButton(exitButton);
                    state->removeButton("Back");
                }
            private:
                GuiAppState *state;
            };
        };

				GameManager *gm = GameManager::getSingleton();
				string font = gm->getPath() + "Fonts/batang.ttf";
				AssetManager::getSingleton()->load(font);
        SpButton *spButton = new SpButton(state, Vector2(gm->getWidth() / 16, gm->getHeight() / 12), Vector2(150, 40), "Singleplayer", true);
        MainMenuOptionsButton *optionsButton = new MainMenuOptionsButton(state, Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 2), Vector2(150, 40), "Options", true);
        ExitButton *exitButton = new ExitButton(Vector2(gm->getWidth() / 16, gm->getHeight() / 12 * 3), Vector2(150, 40));
        state->addButton(spButton);
        state->addButton(optionsButton);
        state->addButton(exitButton);
    }

		Vector2 spaceToScreen(Vector3 pos){
			Root *root = Root::getSingleton();
			Camera *cam = root->getCamera();
			Vector3 dir = cam->getDirection(), up = cam->getUp();
			Vector3 camPos = cam->getPosition();
			mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + dir.x, camPos.y + dir.y, camPos.z + dir.z), vec3(up.x, up.y, up.z));
			
			float fov = cam->getFov(), width = root->getWidth(), height = root->getHeight(), nearPlane = cam->getNearPlane(), farPlane = cam->getFarPlane();
			mat4 proj = perspective(radians(fov), width / height, nearPlane, farPlane);

			vec4 ndcPos = proj * view * vec4(pos.x, pos.y, pos.z, 1);
			ndcPos.x /= ndcPos.w;
			ndcPos.y /= ndcPos.w;
			return Vector2(0.5 * width * (1 + ndcPos.x), 0.5 * height * (1 - ndcPos.y));
		}

		Vector3 screenToSpace(Vector2 pos){
			Root *root = Root::getSingleton();
			Camera *cam = root->getCamera();
			Vector3 dir = cam->getDirection(), up = cam->getUp();
			Vector3 camPos = cam->getPosition();
			mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + dir.x, camPos.y + dir.y, camPos.z + dir.z), vec3(up.x, up.y, up.z));
			
			float fov = cam->getFov(), width = root->getWidth(), height = root->getHeight(), nearPlane = cam->getNearPlane(), farPlane = cam->getFarPlane();
			mat4 proj = perspective(radians(fov), width / height, nearPlane, farPlane);

			mat4 mat = proj * view;
			float w = (mat * vec4(0, 0, 0, 1)).w;
			vec4 ndcPos = vec4(pos.x * 2.0 / width - 1.0, pos.y * -(2.0 / height) + 1.0, -1, 1);
			vec4 spacePos = inverse(mat) * ndcPos;
			spacePos = vec4(spacePos.x / spacePos.w, spacePos.y / spacePos.w, spacePos.z / spacePos.w, spacePos.w);
			return Vector3(spacePos.x, spacePos.y, spacePos.z);
		}
}
