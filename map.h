#pragma once
#ifndef MAP_H
#define MAP_H

#include <irrlicht.h>

namespace game{
    namespace content{
        class Map {
        public:
            Map();
            ~Map();
            void update();
            void load();
            void unload();
            inline irr::core::vector2d<irr::s32> getSize(){return size;}
        private:
            irr::core::vector2d<irr::s32> size;
            irr::scene::ILightSceneNode *sunLight;
            irr::scene::IAnimatedMesh *waterMesh;
            irr::scene::ISceneNode *waterNode;
        };
    }
}

#endif
