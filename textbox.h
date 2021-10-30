#pragma once
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <irrlicht.h>

#include "button.h"

namespace game{
    namespace gui {
        class Textbox {
        public:
            Textbox(irr::core::vector2d<s32>, irr::core::vector2d<s32>);
            ~Textbox();
            void update();
            void enable();
            void disable();
            void type(wchar_t);
            void moveCursor(bool);
            void deleteCharacter();
            virtual irr::core::stringw getEntry(){return entry;}
            virtual void setEntry(irr::core::stringw e){this->entry=e;}
            virtual bool isEnabled(){return enabled;}
            virtual bool isCapitalLeters(){return capitalLeters;}
            virtual void setIsCapitalLeters(bool c){this->capitalLeters=c;}
            virtual void setFont(irr::gui::IGUIFont *f){this->font=f;}
        private:
            class TextboxButton : public Button {
            public:
                TextboxButton(Textbox*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                Textbox *textbox;
            };

            irr::gui::IGUIFont *font;
            irr::core::vector2d<s32> pos, size;
            irr::core::stringw entry = "";
            virtual bool canChangeCursor(){return util::getTime() - lastBlinkTime > 250;}
            bool enabled, canShowCursor = false, capitalLeters = false;
            TextboxButton *textboxButton;
            s64 lastBlinkTime, cursorPosOffset = 0;
        public:
            inline TextboxButton* getTextboxButton(){return textboxButton;}
        };
    }
}

#endif
