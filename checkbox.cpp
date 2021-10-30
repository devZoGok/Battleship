#include "checkbox.h"
#include "defConfigs.h"

using namespace irr::core;
using namespace irr::video;
using namespace game::core;

namespace game{
    namespace gui{
        Checkbox::CheckboxButton::CheckboxButton(Checkbox *ch, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) :Button(pos, size, name, separate) {
            checkbox = ch;
        }
        
        void Checkbox::CheckboxButton::onClick() {
            checkbox->check();
        }

        Checkbox::Checkbox(vector2d<s32> pos){
            this->pos = pos;
            checkboxButton = new CheckboxButton(this, pos, vector2d<s32>(length,length), "CheckboxButton", false);
        }
        
        Checkbox::~Checkbox() {

        }
        
        void Checkbox::update() {
            IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
            driver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(pos.X, pos.Y, pos.X + length, pos.Y + length), nullptr);
            if (checked) {
                driver->draw2DLine(pos, vector2d<s32>(pos.X + length, pos.Y + length),SColor(255,255,255,255));
                driver->draw2DLine(vector2d<s32>(pos.X,pos.Y+length), vector2d<s32>(pos.X + length, pos.Y), SColor(255, 255, 255, 255));
            }
        }
    }
}
