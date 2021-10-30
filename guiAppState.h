#pragma once
#ifndef GUI_APP_STATE_H
#define GUI_APP_STATE_H

#include "gameManager.h"
#include "button.h"
#include "listbox.h"
#include "checkbox.h"
#include "textbox.h"
#include "slider.h"
#include "tooltip.h"
#include <vector>

namespace game{
    namespace core{
        class GuiAppState : public AbstractAppState {
        public:
            GuiAppState();
            ~GuiAppState();
            void onAttachment();
            void onDetachment();
            void update();
            void addButton(gui::Button*);
            void removeButton(gui::Button*);
            void removeSeparateButton(gui::Button*);
            void removeButton(irr::core::stringw);
            void addListbox(gui::Listbox*);
            void removeListbox(gui::Listbox*);
            void addCheckbox(gui::Checkbox*);
            void removeCheckbox(gui::Checkbox*);
            void addTextbox(gui::Textbox*);
            void removeTextbox(gui::Textbox*);
            void addSlider(gui::Slider*);
            void addTooltip(gui::Tooltip*);
            void removeTooltip(gui::Tooltip*);
            void removeSlider(gui::Slider*);
            void removeAllButtons();
            void removeAllListboxes();
            void removeAllCheckboxes();
            void removeAllSliders();
            void removeAllTextboxes();
            void removeAllTooltips();
            inline bool isLeftMousePressed(){return leftMousePressed;}
        private:
            virtual void onAction(Bind, bool);
            virtual void onAnalog(Bind, double);
            virtual void onRawKeyPress(irr::SEvent::SKeyInput);
            virtual void onRawMousePress(irr::SEvent::SMouseInput);
            gui::Textbox* getOpenTextbox();
            gui::Listbox* getOpenListbox();
            void attachBindKeys();
            void attachKeyboardKeys();
            void checkKeyboard(gui::Textbox*, Bind, bool);
            void updateControlsListbox(int);
            
            std::vector<gui::Button*> buttons;
            std::vector<gui::Listbox*> listboxes;
            std::vector<gui::Checkbox*> checkboxes;
            std::vector<gui::Textbox*> textboxes;
            std::vector<gui::Slider*> sliders;
            std::vector<gui::Tooltip*> tooltips;
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
}

#endif
