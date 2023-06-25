#ifndef CONCRETE_GUI_MANAGER_H
#define CONCRETE_GUI_MANAGER_H

#include <abstractGuiManager.h>

#include <string>

namespace battleship{
	enum GuiElementType {BUTTON, LISTBOX, CHECKBOX, SLIDER, TEXTBOX};
	enum ButtonType {SINGLE_PLAYER, EDITOR, OPTIONS, EXIT};

	class ConcreteGuiManager : public vb01Gui::AbstractGuiManager{
		public:
			static ConcreteGuiManager* getSingleton();
			void readLuaScreenScript(std::string);
		private:
			ConcreteGuiManager(){}
			vb01Gui::Button* parseButton(int);
			vb01Gui::Listbox* parseListbox(int);
			vb01Gui::Checkbox parseCheckbox(int);
			vb01Gui::Slider* parseSlider(int);
			vb01Gui::Textbox* parseTextbox(int);
	};
}

#endif
