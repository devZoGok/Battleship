#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "defConfigs.h"
#include "abstractAppState.h"
#include "key.h"
#include "util.h"
#include <vector>
#include <irrlicht.h>
#include <string>

namespace game{
    namespace gui{
        class AbstractBitmapText;
        class AbstractImage;
    }

    typedef gui::AbstractBitmapText BitmapText;
    typedef gui::AbstractImage Image;

    namespace core{
        class EventListener;
        class StateManager;
        
        class GameManager {
        public:
						static GameManager* getSingleton();
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
            inline int getWidth(){return width;}
            inline int getHeight(){return height;}
            inline EventListener* getListener(){return listener;}
            inline bool isServerSide(){return serverSide;}
						inline StateManager* getStateManager(){return stateManager;}
        private:
            GameManager();
            ~GameManager();

            irr::IrrlichtDevice *device = nullptr;
						StateManager *stateManager = nullptr;
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
