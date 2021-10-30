#pragma once
#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "button.h"

namespace game{
    namespace gui {
        class Checkbox {
        public:
            Checkbox(irr::core::vector2d<s32>);
            ~Checkbox();
            void update();
            virtual void check(){checked=!checked;}
            virtual bool isChecked(){return checked;}
        private:
            class CheckboxButton : public Button {
            public:
                CheckboxButton(Checkbox*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                Checkbox *checkbox = nullptr;
            };
            const int length=15;
            bool checked = false;
            irr::core::vector2d<s32> pos;
            CheckboxButton *checkboxButton;
        public:
            inline CheckboxButton* getCheckboxButton(){return checkboxButton;}
        };
    }
}

#endif
