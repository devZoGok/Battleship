#pragma once
#ifndef LISTBOX_H
#define LISTBOX_H

#include "button.h"

namespace game{
    namespace gui {
        class Listbox {
        public:
            Listbox(core::GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, std::vector<irr::core::stringw>, int,bool=false);
            ~Listbox();
            void update();
            void openUp();
            void close();
            void scrollUp();
            void scrollDown();
            inline void addLine(irr::core::stringw l){lines.push_back(l);}
            inline void changeLine(int i, irr::core::stringw l){lines[i]=l;}
            inline bool isOpen(){return open;}
            void appendLines(std::vector<irr::core::stringw>&);
            inline std::vector<irr::core::stringw> getContents(){return lines;}
            inline int getSelectedOption(){return selectedOption;}
        private:
            class ListboxButton : public Button {
            public:
                ListboxButton(core::GameManager*, Listbox*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                Listbox *listbox = nullptr;
            };
            class ScrollingButton : public Button {
            public:
                ScrollingButton(core::GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
            };
            int maxDisplay, scrollOffset = 0, selectedOption = 0;
            bool open = false,controlsListbox=false;
            std::vector<irr::core::stringw> lines;
            irr::core::vector2d<s32> pos, size;
            ListboxButton *listboxButton = nullptr;
            Button *scrollingButton = nullptr;
            core::GameManager *gameManager;
        public:
            inline ListboxButton* getListboxButton(){return listboxButton;}
            inline bool isControlsListbox(){return controlsListbox;}
            inline irr::core::stringw getLine(int i){return lines[i];}
        };
    }
}

#endif
