#include "tooltip.h"

namespace game{
    namespace gui{
        using namespace irr::core;
        using namespace irr::video;
        using namespace game::core;
        
        Tooltip::Tooltip(vector2di pos, stringw entry){
            this->pos = pos;
						GameManager *gm = GameManager::getSingleton();
            text=new BitmapText(entry,pos,gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fontcourier.bmp"));
            gm->attachBitmapText(text);
        }
        
        Tooltip::~Tooltip(){
						GameManager::getSingleton()->detachBitmapText(text);
        }
        
        void Tooltip::update(){
            IVideoDriver *driver=GameManager::getSingleton()->getDevice()->getVideoDriver();
            dimension2d<u32> dim=text->getFont()->getDimension(text->getText().c_str());
            driver->draw2DRectangle(SColor(255,0,0,0),recti(pos.X,pos.Y,pos.X+dim.Width,pos.Y+dim.Height),nullptr);
            driver->draw2DRectangleOutline(recti(pos-vector2di(1,1),dimension2df(dim.Width+1,dim.Height+1)),SColor(255,255,0,0));
        }
    }
}
