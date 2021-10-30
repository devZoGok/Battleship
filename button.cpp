#include "button.h"
#include "defConfigs.h"

using namespace irr::core;
using namespace irr::video;
using namespace irr::gui;
using namespace game::core;
using namespace game::core;

namespace game{
    namespace gui{
        Button::Button() {}

        Button::Button(vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) {
            color = new SColor(255, 200, 200, 200);
            this->name = name;
            this->pos = pos;
            this->size = size;
            this->separate = separate;
        }

        Button::~Button() {
            delete color;
            color = nullptr;
        }

        void Button::update() {
						GameManager *gm = GameManager::getSingleton();
            IVideoDriver *driver = gm->getDevice()->getVideoDriver();

            if (!imageButton) {
                driver->draw2DRectangle(*color, rect<s32>(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y), nullptr);
                if (separate) {
                    IGUIFont *font = gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fontcourier.bmp");
                    font->draw(name, rect<s32>(pos.X + size.X / 4 + textXOffset, pos.Y + size.Y / 4 + textYOffset, size.X, size.Y), SColor(255, 250, 250, 250));
                }
            } else
                driver->draw2DImage(image->getImage(), pos);
        }

        void Button::moveButton(int x, int y) {
            pos.X += x;
            pos.Y += y;
            moveText(x, y);
        }

        void Button::moveText(int x, int y) {
            textXOffset += x;
            textYOffset += y;
        }

        void Button::setImageButton(Image *i) {
            imageButton = true;
            image = i;
            image->setPos(pos);
            size = image->getSize();
        }
    }
}
