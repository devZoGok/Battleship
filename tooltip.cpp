#include "tooltip.h"

using namespace std;
using namespace vb01;

namespace battleship{
    Tooltip::Tooltip(Vector2 pos, string entry){
        this->pos = pos;
				/*
				GameManager *gm = GameManager::getSingleton();
        text=new BitmapText(entry,pos,gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fontcourier.bmp"));
        gm->attachBitmapText(text);
				*/
    }
    
    Tooltip::~Tooltip(){
				//GameManager::getSingleton()->detachBitmapText(text);
    }
    
    void Tooltip::update(){
				/*
        IVideoDriver *driver=GameManager::getSingleton()->getDevice()->getVideoDriver();
        dimension2d<u32> dim=text->getFont()->getDimension(text->getText().c_str());
        driver->draw2DRectangle(SColor(255,0,0,0),recti(pos.X,pos.Y,pos.X+dim.Width,pos.Y+dim.Height),nullptr);
        driver->draw2DRectangleOutline(recti(pos-vector2di(1,1),dimension2df(dim.Width+1,dim.Height+1)),SColor(255,255,0,0));
				*/
    }
}
