#pragma once
#ifndef MAP_H
#define MAP_H

#include <irrlicht.h>
#include "gameManager.h"

namespace game{
    namespace content{
        class Map {
        public:
            Map(core::GameManager*);
            ~Map();
            void update();
            void load();
            void unload();
            inline irr::core::vector2d<s32> getSize(){return size;}
        private:
            core::GameManager *gameManager;
            irr::core::vector2d<s32> size;
            irr::scene::ILightSceneNode *sunLight;
            irr::scene::IAnimatedMesh *waterMesh;
            irr::scene::ISceneNode *waterNode;
        };
    }
}

#endif
