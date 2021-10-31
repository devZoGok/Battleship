#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <text.h>

namespace game{
    namespace gui{
        class Tooltip{
        public:
            Tooltip(vb01::Vector2, std::string);
            ~Tooltip();
            void update();
        private:
            vb01::Vector2 pos;
            //BitmapText *text;
        };
    }
}

#endif
