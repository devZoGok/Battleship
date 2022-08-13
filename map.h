#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>

#include <vector.h>

namespace gameBase{
	class LuaManager;
}

namespace battleship{
		struct GraphNode{
				enum Type{LAND, SEA, AIR};

				Type type;
				vb01::Vector3 pos;
		};

    class Map {
    public:
        Map(std::string);
        ~Map();
        void update();
        void load();
        void unload();
		inline vb01::Node* getNodeParent(){return nodeParent;}
    private:
		vb01::Node *nodeParent = nullptr;
		std::string mapName;
		float cellSize = 1;
		GraphNode*** cells = nullptr;
		int **weights = nullptr;

		void loadSkybox(gameBase::LuaManager*);
		void loadTerrain(gameBase::LuaManager*);
		void loadWaterbodies(gameBase::LuaManager*);
		void generateCells();
    };
}

#endif
