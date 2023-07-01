#ifndef CONCRETE_GUI_MANAGER_H
#define CONCRETE_GUI_MANAGER_H

#include <abstractGuiManager.h>

#include <string>
#include <utility>
#include <vector>

namespace battleship{
	enum GuiElementType {BUTTON, LISTBOX, CHECKBOX, SLIDER, TEXTBOX};
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
		EXPORT
	};
	enum ListboxType {
		CONTROLS,
		RESOLUTION,
		MAPS,
		UNITS,
		SKYBOX_TEXTURES,
		LAND_TEXTURES
	};

	class ConcreteGuiManager : public vb01Gui::AbstractGuiManager{
		public:
			static ConcreteGuiManager* getSingleton();
			void readLuaScreenScript(std::string);
		private:
			ConcreteGuiManager(){}
			vb01Gui::Button* parseButton(int);
			vb01Gui::Listbox* parseListbox(int);
			vb01Gui::Checkbox* parseCheckbox(int);
			vb01Gui::Slider* parseSlider(int);
			vb01Gui::Textbox* parseTextbox(int);

			std::vector<std::pair<int*, void*>> guiElements;
	};
}

#endif
