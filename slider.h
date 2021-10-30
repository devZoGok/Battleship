#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include<irrlicht.h>

#include "button.h"
#include "textbox.h"

namespace game{
    namespace gui{
        class Slider {
        public:
            Slider(irr::core::vector2d<s32>, irr::core::vector2d<s32>, double, double);
            ~Slider();
            void update();
            inline void addTextbox(Textbox *t){this->textbox=t;}
            inline double getMinValue(){return minValue;}
            inline double getValue(){return value;}
            inline double getMaxValue(){return maxValue;}
            inline double getIncrement(){return increment;}
            inline void setValue(double v){this->value=v;}
            inline void setIncrement(double i){this->increment=i;}
            inline irr::core::vector2d<s32> getPos(){return pos;}
            inline irr::core::vector2d<s32> getSize(){return size;}
            inline Textbox* getTextbox(){return textbox;}
        private:
            class MovableSliderButton : public Button {
            public:
                MovableSliderButton(Slider*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
                void update();
            private:
                Slider *slider;
                bool clicked = false;
            };
            class StaticSliderButton : public Button {
            public:
                StaticSliderButton(Slider*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                Slider *slider;
            };
            MovableSliderButton *movableSliderButton = nullptr;
            StaticSliderButton *staticSliderButton = nullptr;
            double minValue, value, maxValue, increment = .1;
            irr::core::vector2d<s32> pos, size;
            Textbox *textbox = nullptr;
        public:
            inline MovableSliderButton* getMovableSliderButton(){return movableSliderButton;}
            inline StaticSliderButton* getStaticSliderButton(){return staticSliderButton;}
        };
    }
}

#endif
