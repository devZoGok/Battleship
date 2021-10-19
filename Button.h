#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include<irrlicht.h>
#include<string>
#include"AbstractImage.h"
#include"AbstractBitmapText.h"
#include"GameManager.h"
#include"util.h"

namespace game{
    namespace gui {
        class Button {
        public:
            Button();
            Button(core::GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
            ~Button();
            virtual void onClick(){}
            virtual void onMouseOver(){}
            virtual void onMouseAway(){}
            virtual void update();
            virtual void moveText(int, int);
            virtual void moveButton(int, int);
            inline void setMouseOverDone(bool m){this->mouseOverDone=m;}
            inline void setMouseAwayDone(bool m){this->mouseAwayDone=m;}
            void setImageButton(Image*);
            inline irr::core::vector2d<s32> getPos(){return pos;}
            inline irr::core::vector2d<s32> getSize(){return size;}
            inline void setPos(irr::core::vector2d<s32> p){this->pos=p;}
            inline void setSize(irr::core::vector2d<s32> s){this->size=s;}
            inline irr::video::SColor* getColor(){return color;}
            inline void setColor(irr::video::SColor *c){this->color=c;}
            inline irr::core::stringw getName(){return name;}
            inline void setName(irr::core::stringw n){this->name=n;}
            inline bool isSeparate(){return separate;}
            inline bool isImageButton(){return imageButton;}
            inline bool isMouseOverDone(){return mouseOverDone;}
            inline bool isMouseAwayDone(){return mouseAwayDone;}
            inline Image* getImage(){return image;}
        private:
            Image *image = nullptr;
            bool separate = true, imageButton = false;
            int textXOffset = 0, textYOffset = 0;
        protected:
            irr::core::stringw name;
            irr::core::vector2d<s32> pos, size;
            core::GameManager *gameManager = nullptr;
            irr::video::SColor *color = nullptr;
            bool mouseOverDone=false,mouseAwayDone=false;
        };
    }
}

#endif
