#include"Checkbox.h"
#include"DefConfigs.h"

using namespace irr::core;
using namespace irr::video;
using namespace game::core;

namespace game{
    namespace gui{
        Checkbox::CheckboxButton::CheckboxButton(GameManager *gM, Checkbox *ch, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) :Button(gM, pos, size, name, separate) {
            checkbox = ch;
        }
        
        void Checkbox::CheckboxButton::onClick() {
            checkbox->check();
        }

        Checkbox::Checkbox(GameManager *gM,vector2d<s32> pos){
            gameManager = gM;
            this->pos = pos;
            checkboxButton = new CheckboxButton(gM, this, pos, vector2d<s32>(length,length), "CheckboxButton", false);
        }
        
        Checkbox::~Checkbox() {

        }
        
        void Checkbox::update() {
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            driver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(pos.X, pos.Y, pos.X + length, pos.Y + length), nullptr);
            if (checked) {
                driver->draw2DLine(pos, vector2d<s32>(pos.X + length, pos.Y + length),SColor(255,255,255,255));
                driver->draw2DLine(vector2d<s32>(pos.X,pos.Y+length), vector2d<s32>(pos.X + length, pos.Y), SColor(255, 255, 255, 255));
            }
        }
    }
}
