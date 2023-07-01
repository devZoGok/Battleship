#include "loadMapButton.h"
#include "concreteGuiManager.h"
#include "gameManager.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;

	LoadMapButton::OkButton::OkButton(vb01::Vector2 pos, vb01::Vector2 size, vb01Gui::Listbox *listbox) : Button(pos, size, "Ok", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){ this->listbox = listbox; }

	void LoadMapButton::OkButton::onClick(){
		/*
		string name = wstringToString(listbox->getContents()[listbox->getSelectedOption()]);
		
		StateManager *sm = GameManager::getSingleton()->getStateManager();
		GuiAppState *guiState = (GuiAppState*)sm->getAppStateByType((int)AppStateType::GUI_STATE);
		guiState->removeAllListboxes();
		guiState->removeAllButtons(vector<Button*>{this});
		
		sm->attachAppState(new MapEditorAppState(name, Vector2::VEC_ZERO, false));
		
		guiState->removeButton(this);
		 */
	}

	LoadMapButton::LoadMapButton(Vector2 pos, Vector2 size) : Button(pos, size, "Load map", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){}

	void LoadMapButton::onClick(){
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		guiManager->removeAllButtons();
		guiManager->removeAllListboxes();
		guiManager->removeAllCheckboxes();
		guiManager->removeAllSliders();
		guiManager->removeAllTextboxes();
		guiManager->readLuaScreenScript("loadMap.lua");
		/*
	
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
		 */
	}
}
