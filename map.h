#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>

#include <vector.h>

namespace gameBase{
	class LuaManager;
}

namespace battleship{
    class Map {
    public:
        Map(std::string);
        ~Map();
        void update();
        void load();
        void unload();
				inline vb01::Node* getWaterNode(){return waterNode;}
    private:
				vb01::Node *waterNode = nullptr;
		std::string mapName;

		void loadSkybox(gameBase::LuaManager*);
		void loadTerrain(gameBase::LuaManager*);
		void loadWaterbodies(gameBase::LuaManager*);
    };
}

#endif
