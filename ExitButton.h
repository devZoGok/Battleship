#pragma once
#ifndef EXIT_BUTTON_H
#define EXIT_BUTTON_H

#include"Button.h"

namespace game
{
    namespace gui {
        class ExitButton : public Button {
        public:
            ExitButton(core::GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
            void onClick();
        };
    }
}

#endif
