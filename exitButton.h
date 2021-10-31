#pragma once
#ifndef EXIT_BUTTON_H
#define EXIT_BUTTON_H

#include <button.h>
#include <vector.h>

namespace game
{
    namespace gui {
        class ExitButton : public vb01Gui::Button {
        public:
            ExitButton(vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        };
    }
}

#endif
