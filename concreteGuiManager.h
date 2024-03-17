#ifndef CONCRETE_GUI_MANAGER_H
#define CONCRETE_GUI_MANAGER_H

#include <abstractGuiManager.h>

#include <string>
#include <utility>
#include <vector>

namespace vb01{
	class Node;
	class Text;
}

namespace battleship{
	enum GuiElementType {BUTTON, LISTBOX, CHECKBOX, SLIDER, TEXTBOX, GUI_RECTANGLE, TEXT};
	enum ButtonType {
		SINGLE_PLAYER,
	   	EDITOR,
	   	OPTIONS,
	   	EXIT,
		OK,
		DEFAULTS,
		BACK,
		CONTROLS_TAB,
		MOUSE_TAB,
		VIDEO_TAB,
		AUDIO_TAB,
		MULTIPLAYER_TAB,
		NEW_MAP,
		NEW_MAP_OK,
		LOAD_MAP,
		LOAD_MAP_OK,
		EXPORT,
		PLAY,
		RESUME,
		CONSOLE_SCREEN,
		MAIN_MENU,
		CONSOLE_COMMAND_OK,
		BUILD,
		LAND_FACTORY_TRAIN,
		NAVAL_FACTORY_TRAIN,
		FORT_TRAIN,
		STATISTICS,
		RESEARCH,
		BUY_REFINEDS,
		SELL_REFINEDS,
		BUY_RESEARCH,
		SELL_RESEARCH,
		ACTIVE_GAME_STATE
	};
	enum ListboxType {
		CONTROLS,
		RESOLUTION,
		MAPS,
		VEHICLES,
		STRUCTURES,
		RESOURCE_DEPOSITS,
		SKYBOX_TEXTURES,
		LAND_TEXTURES,
		CPU_DIFFICULTIES,
		FACTIONS,
		CONSOLE
	};

	class ConcreteGuiManager : public vb01Gui::AbstractGuiManager{
		public:
			static ConcreteGuiManager* getSingleton();
			void readLuaScreenScript(
					std::string,
					std::vector<vb01Gui::Button*> = std::vector<vb01Gui::Button*>{},
					std::vector<vb01Gui::Listbox*> = std::vector<vb01Gui::Listbox*>{},
					std::vector<vb01Gui::Checkbox*> = std::vector<vb01Gui::Checkbox*>{},
					std::vector<vb01Gui::Slider*> = std::vector<vb01Gui::Slider*>{},
					std::vector<vb01Gui::Textbox*> = std::vector<vb01Gui::Textbox*>{},
					std::vector<vb01::Node*> = std::vector<vb01::Node*>{},
					std::vector<vb01::Text*> = std::vector<vb01::Text*>{}
				);
		private:
			ConcreteGuiManager(){}
			vb01Gui::Button* parseButton(int);
			vb01Gui::Listbox* parseGameObjectListbox();
			vb01Gui::Listbox* parseListbox(int);
			vb01Gui::Checkbox* parseCheckbox(int);
			vb01Gui::Slider* parseSlider(int);
			vb01Gui::Textbox* parseTextbox(int);
			vb01::Node* parseGuiRectangle(int);
			vb01::Text* parseText(int);

			std::vector<std::pair<int*, void*>> guiElements;
	};
}

#endif
