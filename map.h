#pragma once
#ifndef MAP_H
#define MAP_H

#include <vector.h>

namespace battleship{
    class Map {
    public:
        Map();
        ~Map();
        void update();
        void load();
        void unload();
        inline vb01::Vector2 getSize(){return size;}
    private:
        vb01::Vector2 size;
				/*
        irr::scene::ILightSceneNode *sunLight;
        irr::scene::IAnimatedMesh *waterMesh;
        irr::scene::ISceneNode *waterNode;
				*/
    };
}

#endif
