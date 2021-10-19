#ifndef ABSTRACT_IMAGE
#define ABSTRACT_IMAGE

#include"GameManager.h"
#include"util.h"

namespace game
{
    namespace gui{
        class AbstractImage {
        public:
            AbstractImage(core::GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>);
            ~AbstractImage();
            void update();
            inline irr::video::ITexture* getImage(){return image;}
            inline void setImage(irr::video::ITexture *t){this->image=t;}
            inline irr::core::vector2d<s32> getPos(){return pos;}
            inline irr::core::vector2d<s32> getSize(){return size;}
            inline void setPos(irr::core::vector2d<s32> p){this->pos=p;}
            inline void setSize(irr::core::vector2d<s32> s){this->size=s;}
            inline irr::core::stringw getPath(){return path;}
        private:
            core::GameManager *gameManager;
            irr::video::ITexture *image;
            irr::core::vector2d<s32> pos, size;
            irr::core::stringw path;
        };
    }
}

#endif
