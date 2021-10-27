#ifndef ABSTRACT_BITMAP_TEXT
#define ABSTRACT_BITMAP_TEXT

#include "gameManager.h"

namespace game{
    namespace gui{
        class AbstractBitmapText {
        public:
            AbstractBitmapText(core::GameManager*, irr::core::stringw, irr::core::vector2d<s32>, irr::gui::IGUIFont*);
            ~AbstractBitmapText();
            void update();
            inline irr::core::vector2d<s32> getPos(){return pos;}
            inline void setPos(irr::core::vector2d<s32> p){this->pos=p;}
            inline irr::video::SColor* getColor(){return color;}
            inline void setColor(irr::video::SColor *c){this->color=c;}
            inline irr::core::stringw getText(){return text;}
            inline void setText(irr::core::stringw t){this->text=t;}
            inline irr::gui::IGUIFont* getFont(){return font;}
            inline void setFont(irr::gui::IGUIFont *f){this->font=f;}
        private:
            core::GameManager *gameManager;
            irr::core::stringw text;
            irr::gui::IGUIFont *font;
            irr::core::vector2d<s32> pos;
            irr::video::SColor *color;
        };
    }
}

#endif
