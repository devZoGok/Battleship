#pragma once
#ifndef GUI_APP_STATE_H
#define GUI_APP_STATE_H

#include <button.h>
#include <listbox.h>
#include <checkbox.h>
#include <textbox.h>
#include <slider.h>
#include <vector>

#include "abstractAppState.h"
#include "tooltip.h"
#include "key.h"

namespace battleship{
    class GuiAppState : public AbstractAppState {
    public:
        GuiAppState();
        ~GuiAppState();
        void onAttachment();
        void onDetachment();
        void update();
        void addButton(vb01Gui::Button*);
        void removeButton(vb01Gui::Button*);
        void removeSeparateButton(vb01Gui::Button*);
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
        void removeAllButtons();
        void removeAllListboxes();
        void removeAllCheckboxes();
        void removeAllSliders();
        void removeAllTextboxes();
        void removeAllTooltips();
        inline bool isLeftMousePressed(){return leftMousePressed;}
    private:
        virtual void onAction(Mapping::Bind, bool);
        virtual void onAnalog(Mapping::Bind, double);
				/*
        virtual void onRawKeyPress(irr::SEvent::SKeyInput);
        virtual void onRawMousePress(irr::SEvent::SMouseInput);
				*/
        vb01Gui::Textbox* getOpenTextbox();
        vb01Gui::Listbox* getOpenListbox();
        void attachBindKeys();
        void attachKeyboardKeys();
        void checkKeyboard(vb01Gui::Textbox*, Mapping::Bind, bool);
        void updateControlsListbox(int);
        
        std::vector<vb01Gui::Button*> buttons;
        std::vector<vb01Gui::Listbox*> listboxes;
        std::vector<vb01Gui::Checkbox*> checkboxes;
        std::vector<vb01Gui::Textbox*> textboxes;
        std::vector<vb01Gui::Slider*> sliders;
        std::vector<Tooltip*> tooltips;
        bool leftMousePressed = false, shiftPressed = false;
        const static int numKeys=36;
        const char keyChars[numKeys]{
            '0',
            '1',
            '2',
            '3',
            '4',
            '5',
            '6',
            '7',
            '8',
            '9',
            'a',
            'b',
            'c',
            'd',
            'e',
            'f',
            'g',
            'h',
            'i',
            'j',
            'k',
            'l',
            'm',
            'n',
            'o',
            'p',
            'q',
            'r',
            's',
            't',
            'u',
            'v',
            'w',
            'x',
            'y',
            'z'
        };
    protected:
    };
}

#endif
