#include <luaManager.h>

#include "concreteGuiManager.h"
#include "gameManager.h"
#include "singlePlayerButton.h"
#include "mapEditorButton.h"
#include "optionsButton.h"
#include "exitButton.h"
#include "tabButton.h"
#include "okButton.h"
#include "defaultsButton.h"
#include "backButton.h"
#include "newMapButton.h"
#include "loadMapButton.h"

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
			case OK:
				button = new OkButton(pos, size, name);
				break;
			case DEFAULTS:
				button = new DefaultsButton(pos, size, name);
				break;
			case BACK: {
				string screen = lm->getStringFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("screen")});
				button = new BackButton(pos, size, name, screen);
				break;
			}
			case CONTROLS_TAB:
			case MOUSE_TAB:
			case VIDEO_TAB:
			case AUDIO_TAB:
			case MULTIPLAYER_TAB: {
				string screens[]{
					"controlsTab.lua",
					"mouseTab.lua",
					"videoTab.lua",
					"audioTab.lua",
					"multiplayerTab.lua"
				};
				int diff = ((int)type - (int)CONTROLS_TAB);
				button = new TabButton(pos, size, name, screens[diff]);
				break;
			}
			case NEW_MAP:
				button = new NewMapButton(pos, size);
				break;
			case NEW_MAP_OK:
				button = new NewMapButton::OkButton(pos, size, nullptr, nullptr, nullptr);
				break;
			case LOAD_MAP:
				button = new LoadMapButton(pos, size);
				break;
			case LOAD_MAP_OK:
				button = new LoadMapButton::OkButton(pos, size, nullptr);
				break;
		}

		return button;
	}

	Listbox* createListbox(int guiId){
		LuaManager *lm = LuaManager::getSingleton();

		string guiTable = "gui", posTable = "pos", sizeTable = "size";
		float posX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("x")});
		float posY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("y")});
		Vector2 pos = Vector2(posX, posY);

		float sizeX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("x")});
		float sizeY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("y")});
		Vector2 size = Vector2(sizeX, sizeY);

		int numMaxDisplay = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("numMaxDisplay")});
		ListboxType listboxType = (ListboxType)lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("listboxType")});

		vector<string> lines;
		int maxDisplay, numLines;
		bool closable;

		switch(listboxType){
			case CONTROLS:{
				numLines = 6;
				closable = false;

				for(int i = 0; i < numLines; i++)
					lines.push_back(to_string(i));
			}
				break;
			case RESOLUTION:{
				numLines = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("numLines")});

				for(int i = 0; i < numLines; i++)
					lines.push_back(lm->getStringFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("lines"), Index(i + 1)}));

				closable = true;
			}
				break;
			case MAPS:
				lines = readDir(GameManager::getSingleton()->getPath() + "Models/Maps/", true);
				numLines = lines.size();
				closable = true;
				break;
		}

		maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);
		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/batang.ttf";
		Listbox *listbox = new Listbox(pos, size, lines, maxDisplay, fontPath, closable);

		return listbox;
	}

	Checkbox* createCheckbox(int guiId){
		LuaManager *lm = LuaManager::getSingleton();

		string guiTable = "gui", posTable = "pos", sizeTable = "size";
		float posX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("x")});
		float posY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("y")});
		Vector2 pos = Vector2(posX, posY);

		string fontFile = "batang.ttf";
		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/" + fontFile;
		Checkbox *checkbox = new Checkbox(pos, fontPath);
		return checkbox;
	}

	Slider* createSlider(int guiId){
		LuaManager *lm = LuaManager::getSingleton();

		string guiTable = "gui", posTable = "pos", sizeTable = "size";
		float posX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("x")});
		float posY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("y")});
		Vector2 pos = Vector2(posX, posY);

		float sizeX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("x")});
		float sizeY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("y")});
		Vector2 size = Vector2(sizeX, sizeY);

		float minVal = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("minValue")});
		float maxVal = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("maxValue")});

		Slider *slider = new Slider(pos, size, minVal, maxVal);
		return slider;
	}

	Textbox* createTextbox(int guiId){
		LuaManager *lm = LuaManager::getSingleton();

		string guiTable = "gui", posTable = "pos", sizeTable = "size";
		float posX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("x")});
		float posY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("y")});
		Vector2 pos = Vector2(posX, posY);

		float sizeX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("x")});
		float sizeY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(sizeTable), Index("y")});
		Vector2 size = Vector2(sizeX, sizeY);

		string fontFile = "batang.ttf";
		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/" + fontFile;
		Textbox *textbox = new Textbox(pos, size, fontPath);

		return textbox;
	}

	void ConcreteGuiManager::readLuaScreenScript(string script){
		LuaManager *lm = LuaManager::getSingleton();
		string basePath = GameManager::getSingleton()->getPath() + "Scripts/Gui/";
		lm->buildScript(vector<string>{basePath + "main.lua", basePath + script});

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
