#include "mapEditorButton.h"
#include "guiAppState.h"
#include "inGameAppState.h"
#include "mapEditorAppState.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	void MapEditorButton::onClick(){
		/*
		class NewMapButton : public Button{
			public:
				NewMapButton(Vector2 pos, Vector2 size) : Button(pos, size, "New map", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){}
				void onClick(){
					class OkButton : public Button{
						public:
							OkButton(Vector2 pos, Vector2 size, Textbox *name, Textbox *sx, Textbox *sy) : Button(pos, size, "Ok", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true){
								this->name = name;
								this->sizeX = sx;
								this->sizeY = sy;
							}
							void onClick(){
								GameManager *gm = GameManager::getSingleton();
								StateManager *stateManager = gm->getStateManager();
								GuiAppState *state = (GuiAppState*)stateManager->getAppStateByType((int)AppStateType::GUI_STATE);
		
								state->removeAllTextboxes();
		    					state->removeAllButtons(vector<Button*>{this});
		
								Vector2 size = Vector2(
											atof(wstringToString(sizeX->getText()).c_str()),
											atof(wstringToString(sizeY->getText()).c_str())
										);
								stateManager->attachAppState(new MapEditorAppState(wstringToString(name->getText()), size, true));
		
								state->removeButton(this);
							}
						private:
							Textbox *name, *sizeX, *sizeY;
					};
		
					GameManager *gm = GameManager::getSingleton();
					StateManager *stateManager = gm->getStateManager();
					GuiAppState *state = (GuiAppState*)stateManager->getAppStateByType((int)AppStateType::GUI_STATE);
		    		state->removeAllButtons(vector<Button*>{this});
							
					Vector2 size = Vector2(140, 20);
		
					string fontPath = gm->getPath() + "Fonts/batang.ttf";
					Textbox *name = new Textbox(Vector2(100, 150), size, fontPath);
					Textbox *sizeX = new Textbox(Vector2(100, 150 + (size.y + 10)), size, fontPath);
					Textbox *sizeY = new Textbox(Vector2(100, 150 + 2 * (size.y + 10)), size, fontPath);
					state->addTextbox(name);
					state->addTextbox(sizeX);
					state->addTextbox(sizeY);
		
					state->addButton(new OkButton(Vector2(200, gm->getHeight() - 150), Vector2(140, 50), name, sizeX, sizeY));
		
		    		state->removeButton(this);
				}
			private:
		};
		class LoadMapButton : public Button{
			public:
				LoadMapButton(Vector2 pos, Vector2 size) : Button(pos, size, "Load map", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){}
				void onClick(){
					class OkButton : public Button{
						public:
							OkButton(Vector2 pos, Vector2 size, Listbox *listbox) : Button(pos, size, "Ok", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){
								this->listbox = listbox;
							}

							void onClick(){
								string name = wstringToString(listbox->getContents()[listbox->getSelectedOption()]);

								StateManager *sm = GameManager::getSingleton()->getStateManager();
								GuiAppState *guiState = (GuiAppState*)sm->getAppStateByType((int)AppStateType::GUI_STATE);
								guiState->removeAllListboxes();
								guiState->removeAllButtons(vector<Button*>{this});

								sm->attachAppState(new MapEditorAppState(name, Vector2::VEC_ZERO, false));

								guiState->removeButton(this);
							}
						private:
							Listbox *listbox;
					};

					GameManager *gm = GameManager::getSingleton();
					StateManager *sm = gm->getStateManager();
					GuiAppState *guiState = (GuiAppState*)sm->getAppStateByType((int)AppStateType::GUI_STATE);
					guiState->removeAllButtons(vector<Button*>{this});

					string basePath = gm->getPath();
					vector<string> maps = readDir(basePath + "Models/Maps/", true);
					int displayLimit = 2;
					Listbox *mapsListbox = new Listbox(Vector2(100, 150), Vector2(140, 20), maps, (maps.size() > displayLimit ? maps.size() : displayLimit), basePath + "Fonts/batang.ttf");
					guiState->addListbox(mapsListbox);
					guiState->addButton(new OkButton(Vector2(100, 350), Vector2(100, 20), mapsListbox));

					guiState->removeButton(this);
				}
			private:
		};
		 */
		

		/*
		GameManager *gm = GameManager::getSingleton();
		GuiAppState *guiState = (GuiAppState*)gm->getStateManager()->getAppStateByType((int)AppStateType::GUI_STATE);
		guiState->removeAllButtons(vector<Button*>{this});

		int width = gm->getWidth(), height = gm->getHeight();
		Vector2 size = Vector2(150, 40);

		NewMapButton *newMapButton = new NewMapButton(Vector2(width / 16, height / 12), size);
		LoadMapButton *loadMapButton = new LoadMapButton(Vector2(width / 16, height / 12 * 2), size);
		guiState->addButton(newMapButton);
		guiState->addButton(loadMapButton);
		guiState->removeButton(this);
		*/
	}
}
