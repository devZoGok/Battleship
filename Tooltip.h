#ifndef TOOLTIP_H
#define TOOLTIP_H

#include"AbstractBitmapText.h"

namespace game{
    namespace gui{
        class Tooltip{
        public:
            Tooltip(core::GameManager*,irr::core::vector2di, irr::core::stringw);
            ~Tooltip();
            void update();
        private:
            core::GameManager *gameManager;
            irr::core::vector2di pos;
            BitmapText *text;
        };
    }
}

#endif
