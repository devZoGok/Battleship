#pragma once
#ifndef GUI_APP_STATE_H
#define GUI_APP_STATE_H

#include <button.h>
#include <listbox.h>
#include <checkbox.h>
#include <textbox.h>
#include <slider.h>

#include <vector>

#include <abstractAppState.h>

#include "binds.h"
#include "tooltip.h"

namespace battleship{
    class GuiAppState : public gameBase::AbstractAppState {
    public:
        GuiAppState();
        ~GuiAppState();
        void onAttached();
        void onDettached();
        void update();
        void addButton(vb01Gui::Button*);
        void removeButton(vb01Gui::Button*);
        void removeButton(std::string);
        void addListbox(vb01Gui::Listbox*);
        void removeListbox(vb01Gui::Listbox*);
        void addCheckbox(vb01Gui::Checkbox*);
        void removeCheckbox(vb01Gui::Checkbox*);
        void addTextbox(vb01Gui::Textbox*);
        void removeTextbox(vb01Gui::Textbox*);
        void addSlider(vb01Gui::Slider*);
        void addTooltip(Tooltip*);
        void removeTooltip(Tooltip*);
        void removeSlider(vb01Gui::Slider*);
        void removeAllButtons(std::vector<vb01Gui::Button*> = std::vector<vb01Gui::Button*>{});
        void removeAllListboxes();
        void removeAllCheckboxes();
        void removeAllSliders();
        void removeAllTextboxes();
        void removeAllTooltips();
        inline bool isLeftMousePressed(){return leftMousePressed;}
    private:
		void updateCurrentListbox(vb01Gui::Button*, bool&);
		void updateCurrentSlider(vb01Gui::Button*);
		void updateCurrentTextbox(vb01Gui::Button*, bool&);
        virtual void onAction(int, bool);
        virtual void onAnalog(int, float);
        virtual void onRawKeyPress(int);
		virtual void onRawCharPress(vb01::u32);
        virtual void onRawMousePress(int);
        vb01Gui::Textbox* getOpenTextbox();
        vb01Gui::Listbox* getOpenListbox();
        void updateControlsListbox(int);
        
        std::vector<vb01Gui::Button*> buttons;
        std::vector<vb01Gui::Listbox*> listboxes;
        std::vector<vb01Gui::Checkbox*> checkboxes;
        std::vector<vb01Gui::Textbox*> textboxes;
        std::vector<vb01Gui::Slider*> sliders;
        std::vector<Tooltip*> tooltips;
		vb01Gui::Slider *currentSlider = nullptr;
		vb01Gui::Textbox *currentTextbox = nullptr;
		vb01Gui::Listbox *currentListbox = nullptr;
        bool leftMousePressed = false, backspacePressed = false;
    protected:
    };
}

#endif
