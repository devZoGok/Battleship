#include <luaManager.h>

#include "concreteGuiManager.h"
#include "gameManager.h"
#include "singlePlayerButton.h"
#include "mapEditorButton.h"
#include "optionsButton.h"
#include "exitButton.h"

namespace battleship{
	using namespace std;
	using namespace gameBase;
	using namespace vb01;
	using namespace vb01Gui;

	static ConcreteGuiManager *concreteGuiManager = nullptr;

	ConcreteGuiManager* ConcreteGuiManager::getSingleton(){
		if(!concreteGuiManager)
			concreteGuiManager = new ConcreteGuiManager();

		return concreteGuiManager;
	}

	Button* createButton(int guiId){
		LuaManager *lm = LuaManager::getSingleton();

		string guiTable = "gui", posTable = "pos", sizeTable = "size";
		float posX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("x")});
		float posY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("y")});
		Vector2 pos = Vector2(posX, posY);

		float sizeX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("x")});
		float sizeY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("y")});
		Vector2 size = Vector2(sizeX, sizeY);

		string name = lm->getStringFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("name")});
		ButtonType type = (ButtonType)lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("buttonType")});

		Button *button = nullptr;

		switch(type){
			case SINGLE_PLAYER:
				button = new SinglePlayerButton(pos, size, name);
				break;
			case EDITOR:
				button = new MapEditorButton(pos, size);
				break;
			case OPTIONS:
				button = new OptionsButton(pos, size, name, true);
				break;
			case EXIT:
				button = new ExitButton(pos, size);
				break;
		}

		return button;
	}

	Listbox* createListbox(int guiId){
		Listbox *listbox = nullptr;
		return listbox;
	}

	Checkbox* createCheckbox(int guiId){
		Checkbox *checkbox = nullptr;
		return checkbox;
	}

	Slider* createSlider(int guiId){
		Slider *slider = nullptr;
		return slider;
	}

	Textbox* createTextbox(int guiId){
		Textbox *textbox = nullptr;
		return textbox;
	}

	void ConcreteGuiManager::readLuaScreenScript(string script){
		LuaManager *lm = LuaManager::getSingleton();
		string path = GameManager::getSingleton()->getPath() + "Scripts/Gui/" + script;
		lm->buildScript(vector<string>{path});

		int numGuiElements = lm->getInt("numGui");

		for(int i = 0; i < numGuiElements; i++){
			int guiTypeId = lm->getIntFromTable("gui", vector<Index>{Index(i + 1), Index("guiType")});

			switch((GuiElementType)guiTypeId){
				case BUTTON:
					addButton(createButton(i));
					break;
				case LISTBOX:
					addListbox(createListbox(i));
					break;
				case CHECKBOX:
					addCheckbox(createCheckbox(i));
					break;
				case SLIDER:
					addSlider(createSlider(i));
					break;
				case TEXTBOX:
					addTextbox(createTextbox(i));
					break;
			}
		}
	}
}
