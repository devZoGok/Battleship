#include <solUtil.h>

#include <quad.h>

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
#include "gameObjectListbox.h"
#include "playButton.h"
#include "inGameAppState.h"
#include "mainMenuButton.h"
#include "buildButton.h"
#include "trainButton.h"
#include "statsButton.h"
#include "researchButton.h"
#include "tradeButton.h"
#include "activeStateButton.h"

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
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];

		sol::table posTable = guiTable["pos"];
		Vector2 pos = Vector2(posTable["x"], posTable["y"]);

		sol::table sizeTable = guiTable["size"];
		Vector2 size = Vector2(sizeTable["x"], sizeTable["y"]);

		string nk = "name";
		sol::optional<string> nameOpt = guiTable[nk];
		string name = (nameOpt != sol::nullopt ? (string)guiTable[nk] : "");
		ButtonType type = (ButtonType)guiTable["buttonType"];

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
				string screen = guiTable["screen"];
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
				int numTextboxes = guiTable["numDependencies"];
				vector<Textbox*> t;

				for(int i = 0; i < numTextboxes; i++){
					int tid = guiTable["dependencies"][i + 1]["id"];
					t.push_back((Textbox*)guiElements[tid].second);
				}

				button = new NewMapButton::OkButton(pos, size, t[0], t[1], t[2]);
				break;
			}
			case LOAD_MAP:
				button = new LoadMapButton(pos, size);
				break;
			case LOAD_MAP_OK:{
				int lid = guiTable["dependencies"][1]["id"];
				button = new LoadMapButton::OkButton(pos, size, (Listbox*)guiElements[lid].second);
				break;
			}
			case EXPORT:
				button = new ExportButton(pos, size);
				break;
			case PLAY:{
				int did = guiTable["dependencies"][1]["id"];
				vector<Listbox*> difficultyListboxes = vector<Listbox*>{(Listbox*)guiElements[did].second};

				vector<Listbox*> factionsListboxes;
				int numPlayers = 2;

				for(int i = 0; i < numPlayers; i++){
					int did = guiTable["dependencies"][i + 2]["id"];
					factionsListboxes.push_back((Listbox*)guiElements[did].second);
				}

				int mid = guiTable["dependencies"][4]["id"];
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
				int lid = guiTable["dependencies"][1]["id"];
				Listbox *listbox = (Listbox*)guiElements[lid].second;

				int tid = guiTable["dependencies"][2]["id"];
				Textbox *textbox = (Textbox*)guiElements[tid].second;

				button = new InGameAppState::ConsoleButton::ConsoleCommandEntryButton(textbox, listbox, pos, size, name);
				break;
			}
			case BUILD:
			{
				int unitId = SOL_LUA_STATE["UnitId"]["ENGINEER"];
				int strId = SOL_LUA_STATE["units"][unitId + 1]["buildableUnits"][guiId + 1]["id"];
				string buttonName = SOL_LUA_STATE["units"][strId + 1]["name"];
				button = new BuildButton(pos, size, buttonName, (int)guiTable["trigger"], (string)guiTable["imagePath"], unitId, guiId);
				break;
			}
			case LAND_FACTORY_TRAIN:
			{
				int facId = SOL_LUA_STATE["UnitId"]["LAND_FACTORY"];
				int unitId = SOL_LUA_STATE["units"][facId + 1]["buildableUnits"][guiId + 1]["id"];
				string buttonName = SOL_LUA_STATE["units"][unitId + 1]["name"];
				button = new TrainButton(pos, size, buttonName, (int)guiTable["trigger"], (string)guiTable["imagePath"], facId, guiId);
				break;
			}
			case NAVAL_FACTORY_TRAIN:
			{
				int facId = SOL_LUA_STATE["UnitId"]["NAVAL_FACTORY"];
				int unitId = SOL_LUA_STATE["units"][facId + 1]["buildableUnits"][guiId + 1]["id"];
				string buttonName = SOL_LUA_STATE["units"][unitId + 1]["name"];
				button = new TrainButton(pos, size, buttonName, (int)guiTable["trigger"], (string)guiTable["imagePath"], facId, guiId);
				break;
			}
			case FORT_TRAIN:
			{
				int facId = SOL_LUA_STATE["UnitId"]["FORT"];
				int unitId = SOL_LUA_STATE["units"][facId + 1]["buildableUnits"][guiId + 1]["id"];
				string buttonName = SOL_LUA_STATE["units"][unitId + 1]["name"];
				button = new TrainButton(pos, size, buttonName, (int)guiTable["trigger"], (string)guiTable["imagePath"], facId, guiId);
				break;
			}
			case STATISTICS:
				button = new StatsButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"]);
				break;
			case RESEARCH:
				button = new ResearchButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"], (int)SOL_LUA_STATE["UnitId"]["LAB"], (int)guiTable["techId"]);
				break;
			case BUY_REFINEDS:
				button = new TradeButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"], (int)SOL_LUA_STATE["UnitId"]["TRADE_CENTER"], TradeButton::Type::BUY_REFINEDS);
				break;
			case SELL_REFINEDS:
				button = new TradeButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"], (int)SOL_LUA_STATE["UnitId"]["TRADE_CENTER"], TradeButton::Type::SELL_REFINEDS);
				break;
			case BUY_RESEARCH:
				button = new TradeButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"], (int)SOL_LUA_STATE["UnitId"]["TRADE_CENTER"], TradeButton::Type::BUY_RESEARCH);
				break;
			case SELL_RESEARCH:
				button = new TradeButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"], (int)SOL_LUA_STATE["UnitId"]["TRADE_CENTER"], TradeButton::Type::SELL_RESEARCH);
				break;
			case ACTIVE_GAME_STATE:
				button = new ActiveStateButton(pos, size, name, (int)guiTable["trigger"], (string)guiTable["imagePath"]);
				break;
		}

		int typeArr[2]{(int)GuiElementType::BUTTON, (int)type};
	 	guiElements.push_back(make_pair(typeArr, (void*)button));

		return button;
	}

	Listbox* ConcreteGuiManager::parseGameObjectListbox(){
	}

	Listbox* ConcreteGuiManager::parseListbox(int guiId){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];

		string posTable = "pos";
		Vector2 pos = Vector2(guiTable[posTable]["x"], guiTable[posTable]["y"]);

		string sizeTable = "size";
		Vector2 size = Vector2(guiTable[sizeTable]["x"], guiTable[sizeTable]["y"]);

		int numMaxDisplay = guiTable["numMaxDisplay"];
		ListboxType listboxType = (ListboxType)guiTable["listboxType"];

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
				numLines = guiTable["numLines"];

				for(int i = 0; i < numLines; i++)
					lines.push_back(guiTable["lines"][i + 1]);

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
			case VEHICLES:
			case STRUCTURES:
			case RESOURCE_DEPOSITS:{
				bool resources = (listboxType == RESOURCE_DEPOSITS);
				sol::table gameObjTable = SOL_LUA_STATE[resources ? "resources" : "units"];
				int numGameObjs = gameObjTable["num"];
				
				for(int i = 0; i < numGameObjs; i++){
					bool canAdd = true;

					if(!resources){
						bool vehicles = (listboxType == VEHICLES);
						bool v = gameObjTable["isVehicle"][i + 1];
						canAdd = (v == vehicles);
					}

					if(canAdd)
						lines.push_back(gameObjTable["name"][i + 1]);
				}
				
				numLines = lines.size();
				closable = true;
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);
				
				listbox = new GameObjectListbox(!resources, pos, size, lines, maxDisplay, fontPath);
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
				maxDisplay = (numLines > numMaxDisplay ? numMaxDisplay : numLines);

				listbox = new Listbox(pos, size, lines, maxDisplay, fontPath, closable);
			}
				break;
		}

		int typeArr[2]{(int)GuiElementType::LISTBOX, (int)listboxType};
	 	guiElements.push_back(make_pair(typeArr, (void*)listbox));

		return listbox;
	}

	Checkbox* ConcreteGuiManager::parseCheckbox(int guiId){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];

		string posTable = "pos";
		Vector2 pos = Vector2(guiTable[posTable]["x"], guiTable[posTable]["y"]);

		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/batang.ttf";
		Checkbox *checkbox = new Checkbox(pos, fontPath);

		int typeArr[2]{(int)GuiElementType::CHECKBOX, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)checkbox));

		return checkbox;
	}

	Slider* ConcreteGuiManager::parseSlider(int guiId){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];

		string posTable = "pos", sizeTable = "size";
		Vector2 pos = Vector2(guiTable[posTable]["x"], guiTable[posTable]["y"]);
		Vector2 size = Vector2(guiTable[sizeTable]["x"], guiTable[sizeTable]["y"]);

		Slider *slider = new Slider(pos, size, guiTable["minValue"], guiTable["maxValue"]);

		int typeArr[2]{(int)GuiElementType::SLIDER, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)slider));

		return slider;
	}

	Textbox* ConcreteGuiManager::parseTextbox(int guiId){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];

		string posTable = "pos", sizeTable = "size";
		Vector2 pos = Vector2(guiTable[posTable]["x"], guiTable[posTable]["y"]);
		Vector2 size = Vector2(guiTable[sizeTable]["x"], guiTable[sizeTable]["y"]);

		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/batang.ttf";
		Textbox *textbox = new Textbox(pos, size, fontPath);

		int typeArr[2]{(int)GuiElementType::TEXTBOX, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)textbox));

		return textbox;
	}

	Node* ConcreteGuiManager::parseGuiRectangle(int guiId){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];

		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() + "gui");

		string ipk = "imagePath";
		sol::optional<string> pathOpt = guiTable[ipk];
		string imagePath = (pathOpt != sol::nullopt ? (string)guiTable[ipk] : "");
		bool texturingEnabled = (imagePath != "");

		mat->addBoolUniform("texturingEnabled", texturingEnabled);

		if(texturingEnabled){
			string p[]{GameManager::getSingleton()->getPath() + "Textures/" + (string)guiTable[ipk]};
			Texture *tex = new Texture(p, 1, false);
			mat->addTexUniform("diffuseMap", tex, false);
		}
		else{
			sol::table colorTable = guiTable["color"];
			mat->addVec4Uniform("diffuseColor", Vector4(colorTable["x"], colorTable["y"], colorTable["z"], colorTable["w"]));
		}

		sol::table sizeTable = guiTable["size"];
		Quad *quad = new Quad(Vector3(sizeTable["x"], sizeTable["y"], 1), false);
		quad->setMaterial(mat);

		sol::table posTable = guiTable["pos"];
		Node *guiRectangle = new Node(Vector3(posTable["x"], posTable["y"], guiTable["zIndex"]));
		guiRectangle->attachMesh(quad);
		root->getGuiNode()->attachChild(guiRectangle);

		int typeArr[2]{(int)GuiElementType::GUI_RECTANGLE, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)guiRectangle));

		return guiRectangle;
	}

	Text* ConcreteGuiManager::parseText(int guiId){
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table guiTable = SOL_LUA_STATE["gui"][guiId + 1];
		sol::table posTable = guiTable["pos"];

		Root *root = Root::getSingleton();

		Material *mat = new Material(root->getLibPath() + "text");
		mat->addBoolUniform("texturingEnabled", false);
		sol::table colorTable = guiTable["color"];
		mat->addVec4Uniform("diffuseColor", Vector4(colorTable["x"], colorTable["y"], colorTable["z"], colorTable["w"]));

		Text *text = new Text(GameManager::getSingleton()->getPath() + "Fonts/" + (string)guiTable["font"], (wstring)guiTable["text"], guiTable["fontFirstChar"], guiTable["fontLastChar"]);
		text->setScale((float)guiTable["scale"]);
		text->setMaterial(mat);

		Vector3 pos = Vector3(posTable["x"], posTable["y"], guiTable["zIndex"]);
		Node *node = new Node(pos, Quaternion::QUAT_W, Vector3::VEC_IJK, guiTable["name"]);
		node->addText(text);
		root->getGuiNode()->attachChild(node);

		int typeArr[2]{(int)GuiElementType::TEXT, -1};
	 	guiElements.push_back(make_pair(typeArr, (void*)text));

		return text;
	}

	void ConcreteGuiManager::readLuaScreenScript(
			string script,
			vector<Button*> buttonExceptions,
			vector<Listbox*> listboxExceptions,
			vector<Checkbox*> checkboxExceptions,
			vector<Slider*> sliderExceptions,
			vector<Textbox*> textboxExceptions,
			vector<Node*> guiRecttboxExceptions,
			vector<Text*> textExceptions
		){
		removeAllGuiElements(buttonExceptions, listboxExceptions, checkboxExceptions, sliderExceptions, textboxExceptions, guiRecttboxExceptions, textExceptions);
		guiElements.clear();

		string basePath = GameManager::getSingleton()->getPath() + "Scripts/Gui/";
		sol::state_view SOL_LUA_VIEW = generateView();
		SOL_LUA_VIEW.script_file(basePath + script);

		SOL_LUA_VIEW.script("numGui = #gui");
		int numGuiElements = SOL_LUA_VIEW["numGui"];

		for(int i = 0; i < numGuiElements; i++){
			int guiTypeId = SOL_LUA_VIEW["gui"][i + 1]["guiType"];

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
				case GUI_RECTANGLE:
					addGuiRectangle(parseGuiRectangle(i));
					break;
				case TEXT:
					addText(parseText(i));
					break;
			}
		}
	}
}
