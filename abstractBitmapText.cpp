#include "abstractBitmapText.h"
#include <IGUIFont.h>

using namespace irr::core;
using namespace irr::video;
using namespace irr::gui;
using namespace game::core;

namespace game{
    namespace gui{
        AbstractBitmapText::AbstractBitmapText(stringw text, vector2d<s32> pos, IGUIFont *font) {
            this->text = text;
            this->pos = pos;
            this->font = font;
            color = new SColor(255, 255, 255, 255);
        }

        AbstractBitmapText::~AbstractBitmapText() {}

        void AbstractBitmapText::update() {
            font->draw(text, rect<s32>(pos.X, pos.Y, pos.X + 10, pos.Y + 10), *color);
        }
    }
}
