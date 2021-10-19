#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include"DefConfigs.h"
#include"AbstractAppState.h"
#include"Key.h"
#include"util.h"
#include<vector>
#include<irrlicht.h>
#include<string>

namespace game{
    namespace gui{
        class AbstractBitmapText;
        class AbstractImage;
    }

    typedef gui::AbstractBitmapText BitmapText;
    typedef gui::AbstractImage Image;

    namespace core{
        class EventListener;
        
        class GameManager {
        public:
            GameManager(irr::IrrlichtDevice*);
            ~GameManager();
            void attachState(AbstractAppState*);
            void dettachState(AbstractAppState*);
            void dettachState(AppStateTypes);
            inline void attachBitmapText(gui::AbstractBitmapText *a){bitmapTexts.push_back(a);}
            void detachBitmapText(gui::AbstractBitmapText*);
            void detachAllBitmapTexts();
            inline void attachImage(gui::AbstractImage *a){images.push_back(a);}
            void detachImage(gui::AbstractImage*);
            void detachAllImages();
            void update();
            inline irr::IrrlichtDevice* getDevice(){return device;}
            inline irr::scene::ISceneManager* getSceneManager(){return device->getSceneManager();}
            inline void setDevice(irr::IrrlichtDevice *d){this->device=d;}
            inline AbstractAppState* getAppState(int id){return appStates[id];}
            AbstractAppState* getAppState(AppStateTypes);
            inline void setAppState(int i, AbstractAppState *a){appStates[i]=a;}
            inline int getAppStateNumber(){return appStates.size();}
            inline int getWidth(){return width;}
            inline int getHeight(){return height;}
            inline EventListener* getListener(){return listener;}
            inline bool isServerSide(){return serverSide;}
        private:
            irr::IrrlichtDevice *device = nullptr;
            std::vector<AbstractAppState*> appStates;
            int width, height;
            std::vector<gui::AbstractBitmapText*> bitmapTexts;
            std::vector<gui::AbstractImage*> images;
            EventListener *listener = nullptr;
            bool serverSide;
        };
        
    }
}

#endif
