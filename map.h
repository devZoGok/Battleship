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
				inline vb01::Node* getWaterNode(){return waterNode;}
    private:
        vb01::Vector2 size;
				vb01::Node *waterNode = nullptr;
    };
}

#endif
