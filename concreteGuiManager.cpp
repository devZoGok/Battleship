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
#include "exportButton.h"
#include "skyboxTextureListbox.h"
#include "landTextureListbox.h"
#include "unitListbox.h"
#include "playButton.h"
#include "inGameAppState.h"
#include "mainMenuButton.h"

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

	//TODO refactor player difficulty and faction listbox selection
	Button* ConcreteGuiManager::parseButton(int guiId){
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
			case NEW_MAP_OK:{
				int numTextboxes = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("numDependencies")});
				vector<Textbox*> t;

				for(int i = 0; i < numTextboxes; i++){
					int tid = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(i + 1), Index("id")});
					t.push_back((Textbox*)guiElements[tid].second);
				}

				button = new NewMapButton::OkButton(pos, size, t[0], t[1], t[2]);
				break;
			}
			case LOAD_MAP:
				button = new LoadMapButton(pos, size);
				break;
			case LOAD_MAP_OK:{
				int lid = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(1), Index("id")});
				button = new LoadMapButton::OkButton(pos, size, (Listbox*)guiElements[lid].second);
				break;
			}
			case EXPORT:
				button = new ExportButton(pos, size);
				break;
			case PLAY:{
				int did = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(1), Index("id")});
				vector<Listbox*> difficultyListboxes = vector<Listbox*>{(Listbox*)guiElements[did].second};

				vector<Listbox*> factionsListboxes;
				int numPlayers = 2;

				for(int i = 0; i < numPlayers; i++){
					int fid = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(i + 2), Index("id")});
					factionsListboxes.push_back((Listbox*)guiElements[fid].second);
				}

				int mid = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(4), Index("id")});
				Listbox *mapListbox = (Listbox*)guiElements[mid].second;

				button = new PlayButton(difficultyListboxes, factionsListboxes, mapListbox, pos, size, name, true);
				break;
			}
			case RESUME:
				button = new InGameAppState::ResumeButton(pos, size);
				break;
			case CONSOLE_SCREEN:
				button = new InGameAppState::ConsoleButton(pos, size);
				break;
			case MAIN_MENU:
				button = new MainMenuButton(pos, size, name);
				break;
			case CONSOLE_COMMAND_OK:{
				int lid = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(1), Index("id")});
				Listbox *listbox = (Listbox*)guiElements[lid].second;

				int tid = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("dependencies"), Index(2), Index("id")});
				Textbox *textbox = (Textbox*)guiElements[tid].second;

				button = new InGameAppState::ConsoleButton::ConsoleCommandEntryButton(textbox, listbox, pos, size, name);
				break;
			}
		}

		int typeArr[2]{(int)GuiElementType::BUTTON, (int)type};
	 	guiElements.push_back(make_pair(typeArr, (void*)button));

		return button;
	}

	Listbox* ConcreteGuiManager::parseListbox(int guiId){
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
		Listbox *listbox = nullptr;

		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/batang.ttf";

		switch(listboxType){
			case CONTROLS:{
				numLines = 6;
				closable = false;

				for(int i = 0; i < numLines; i++)
					lines.push_back(to_string(i));

				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);
				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath, closable);
				
				break;
			}
			case RESOLUTION:{
				numLines = lm->getIntFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("numLines")});

				for(int i = 0; i < numLines; i++)
					lines.push_back(lm->getStringFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("lines"), Index(i + 1)}));

				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath, closable);

				break;
			}
			case MAPS:
				lines = readDir(GameManager::getSingleton()->getPath() + "Models/Maps/", true);
				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath, closable);
				break;
			case UNITS:{
				bool vehicles = lm->getBoolFromTable(guiTable, vector<Index>{Index(guiId + 1), Index("vehicles")});
				int numUnits = lm->getInt("numUnits");

				for(int i = 0; i < numUnits; i++){
					bool v = lm->getBoolFromTable("isVehicle", vector<Index>{Index(i + 1)});

					if(v == vehicles){
						string name = lm->getStringFromTable("name", vector<Index>{Index(i + 1)});
						lines.push_back(name);
					}

				}

				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new UnitListbox(pos, size, lines, maxDisplay, fontPath);
				break;
			}
			case SKYBOX_TEXTURES:
				lines = readDir(GameManager::getSingleton()->getPath() + "Textures/Skyboxes", true);
				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new SkyboxTextureListbox(pos, size, lines, maxDisplay, fontPath);
				break;
			case LAND_TEXTURES:
				lines = readDir(GameManager::getSingleton()->getPath() + "Textures/Landmass", false);
				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new LandTextureListbox(pos, size, lines, maxDisplay, fontPath);
				break;
			case CPU_DIFFICULTIES:
				lines = vector<string>{"Easy", "Medium", "Hard"};
				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath);
				break;
			case FACTIONS:
				lines = vector<string>{"Empire", "Mutants", "Shapeshifters"};
				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath);
				break;
			case CONSOLE:{
				for(int i = 0; i < numMaxDisplay; i++)
					lines.push_back("");

				closable = false;

				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath, closable);
			}
				break;
		}

		int typeArr[2]{(int)GuiElementType::LISTBOX, (int)listboxType};
	 	guiElements.push_back(make_pair(typeArr, (void*)listbox));

		return listbox;
	}

	Checkbox* ConcreteGuiManager::parseCheckbox(int guiId){
		LuaManager *lm = LuaManager::getSingleton();

		string guiTable = "gui", posTable = "pos", sizeTable = "size";
		float posX = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("x")});
		float posY = lm->getFloatFromTable(guiTable, vector<Index>{Index(guiId + 1), Index(posTable), Index("y")});
		Vector2 pos = Vector2(posX, posY);

		string fontFile = "batang.ttf";
		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/" + fontFile;
		Checkbox *checkbox = new Checkbox(pos, fontPath);

		int typeArr[2]{(int)GuiElementType::CHECKBOX, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)checkbox));

		return checkbox;
	}

	Slider* ConcreteGuiManager::parseSlider(int guiId){
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

		int typeArr[2]{(int)GuiElementType::SLIDER, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)slider));

		return slider;
	}

	Textbox* ConcreteGuiManager::parseTextbox(int guiId){
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

		int typeArr[2]{(int)GuiElementType::TEXTBOX, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)textbox));

		return textbox;
	}

	void ConcreteGuiManager::readLuaScreenScript(string script){
		removeAllGuiElements();

		guiElements.clear();

		LuaManager *lm = LuaManager::getSingleton();
		string basePath = GameManager::getSingleton()->getPath() + "Scripts/Gui/";
		lm->buildScript(vector<string>{basePath + "main.lua", basePath + script});

		int numGuiElements = lm->getInt("numGui");

		for(int i = 0; i < numGuiElements; i++){
			int guiTypeId = lm->getIntFromTable("gui", vector<Index>{Index(i + 1), Index("guiType")});
			vector<Index> indices = vector<Index>{Index(i + 1)};
			void *element = nullptr;

			switch((GuiElementType)guiTypeId){
				case BUTTON:
					addButton(parseButton(i));
					break;
				case LISTBOX:
					addListbox(parseListbox(i));
					break;
				case CHECKBOX:
					addCheckbox(parseCheckbox(i));
					break;
				case SLIDER:
					addSlider(parseSlider(i));
					break;
				case TEXTBOX:
					addTextbox(parseTextbox(i));
					break;
			}
		}
	}
}
