#include "newMapButton.h"
#include "concreteGuiManager.h"
#include "gameManager.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;

	NewMapButton::OkButton::OkButton(vb01::Vector2 pos, vb01::Vector2 size, vb01Gui::Textbox *name, vb01Gui::Textbox *sx, vb01Gui::Textbox *sy) : Button(pos, size, "Ok", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true){
		this->name = name;
		this->sizeX = sx;
		this->sizeY = sy;
	}

	void NewMapButton::OkButton::onClick(){
		/*
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
		 */
	}

	NewMapButton::NewMapButton(Vector2 pos, Vector2 size) : Button(pos, size, "New map", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){}

	void NewMapButton::onClick(){
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		guiManager->removeAllButtons();
		guiManager->removeAllListboxes();
		guiManager->removeAllCheckboxes();
		guiManager->removeAllSliders();
		guiManager->removeAllTextboxes();
		guiManager->readLuaScreenScript("newMap.lua");
		/*
	
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
		 */
	}
}

