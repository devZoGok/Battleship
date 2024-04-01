#include "activeStateButton.h"
#include "gameManager.h"
#include "activeGameState.h"
#include "concreteGuiManager.h"

#include <listbox.h>
#include <checkbox.h>
#include <slider.h>
#include <textbox.h>

#include <text.h>

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	ActiveStateButton::ActiveStateButton(Vector2 pos, Vector2 size, string gs, string name, int trigger, string imagePath) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, true, imagePath), guiScreen(gs){
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		ActiveGameState *activeState = (ActiveGameState*)stateManager->getAppStateByType((int)AppStateType::ACTIVE_STATE);

		if(activeState)
			activeState->addButton(this);
	}

	void ActiveStateButton::onClick(){
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();

		vector<Listbox*> listboxes{};
		vector<Checkbox*> checkboxes{};
		vector<Slider*> sliders{};
		vector<Textbox*> textboxes{};
		vector<Node*> guiRects = guiManager->getGuiRectangles();
		vector<Text*> texts{
			guiManager->getText("depth"),
			guiManager->getText("refineds"),
			guiManager->getText("wealth"),
			guiManager->getText("research")
		};

		guiManager->readLuaScreenScript(guiScreen, buttons, listboxes, checkboxes, sliders, textboxes, guiRects, texts);
	}
}
