#include "abstractImage.h"

using namespace irr::video;
using namespace irr::core;
using namespace game::core;

namespace game
{
    namespace gui{
        AbstractImage::AbstractImage(ITexture *image, vector2d<s32> pos, vector2d<s32> size) {
            this->image = image;
            this->pos = pos;
            this->size = size;
        }

        AbstractImage::~AbstractImage() {
        }

        void AbstractImage::update() {
            irr::video::IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
            driver->draw2DImage(image, pos);
        }
    }
}
