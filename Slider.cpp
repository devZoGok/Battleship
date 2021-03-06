#include"Slider.h"
#include<algorithm>
#include"DefConfigs.h"
#include"util.h"
#include<sstream>

using namespace irr::core;
using namespace irr::video;
using namespace game::core;

namespace game{
    namespace gui{
        Slider::MovableSliderButton::MovableSliderButton(GameManager *gM, Slider *slider, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            this->slider = slider;
        }

        void Slider::MovableSliderButton::onClick() {
            clicked = !clicked;
        }

        void Slider::MovableSliderButton::update() {
            int posX = gameManager->getDevice()->getCursorControl()->getPosition().X, posY = gameManager->getDevice()->getCursorControl()->getPosition().Y;
            if (clicked && posY > slider->getPos().Y && posY < slider->getPos().Y + slider->getSize().Y) {
                if (posX < slider->getPos().X)
                    posX = slider->getPos().X;
                else if (posX > slider->getPos().X + slider->getSize().X)
                    posX = slider->getPos().X + slider->getSize().X;
                slider->setValue((((double) posX - slider->getPos().X) / slider->getSize().X) * slider->getMaxValue());
            }
        }

        Slider::StaticSliderButton::StaticSliderButton(GameManager *gM, Slider *slider, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            this->slider = slider;
        }

        void Slider::StaticSliderButton::onClick() {
            s16 buttonClickPoint = -slider->getPos().X + gameManager->getDevice()->getCursorControl()->getPosition().X;
            slider->setValue(((double) buttonClickPoint / slider->getSize().X) * slider->getMaxValue());
        }

        Slider::Slider(GameManager *gM, vector2d<s32> pos, vector2d<s32> size, double minValue, double maxValue) {
            gameManager = gM;
            this->pos = pos;
            this->size = size;
            this->minValue = minValue;
            this->maxValue = maxValue;
            staticSliderButton = new StaticSliderButton(gM, this, pos, size, "StaticSliderButton", false);
            movableSliderButton = new MovableSliderButton(gM, this, vector2d<s32>(pos.X + staticSliderButton->getSize().X / 1, pos.Y - 15), vector2d<s32>(10, 40), "MovableSliderButton", false);
            value = (maxValue - minValue) / 2;
        }

        Slider::~Slider() {
        }

        void Slider::update() {
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            if (value < minValue)
                value = minValue;
            else if (value > maxValue)
                value = maxValue;
            movableSliderButton->update();
            driver->draw2DRectangle(*staticSliderButton->getColor(), rect<s32>(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y), nullptr);
            vector2d<s32> p = movableSliderButton->getPos(), s = movableSliderButton->getSize();
            p.X = pos.X + (double) size.X / maxValue * value;
            driver->draw2DRectangle(SColor(255, 200, 200, 200), rect<s32>(p.X, p.Y, p.X + s.X, p.Y + s.Y), nullptr);
            movableSliderButton->setPos(p);
        }
    }
}
