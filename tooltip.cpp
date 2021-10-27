#include "tooltip.h"

namespace game{
    namespace gui{
        using namespace irr::core;
        using namespace irr::video;
        using namespace game::core;
        
        Tooltip::Tooltip(GameManager *gm, vector2di pos, stringw entry){
            gameManager=gm;
            this->pos=pos;
            text=new BitmapText(gm,entry,pos,gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fontcourier.bmp"));
            gameManager->attachBitmapText(text);
        }
        
        Tooltip::~Tooltip(){
            gameManager->detachBitmapText(text);
        }
        
        void Tooltip::update(){
            IVideoDriver *driver=gameManager->getDevice()->getVideoDriver();
            dimension2d<u32> dim=text->getFont()->getDimension(text->getText().c_str());
            driver->draw2DRectangle(SColor(255,0,0,0),recti(pos.X,pos.Y,pos.X+dim.Width,pos.Y+dim.Height),nullptr);
            driver->draw2DRectangleOutline(recti(pos-vector2di(1,1),dimension2df(dim.Width+1,dim.Height+1)),SColor(255,255,0,0));
        }
    }
}
