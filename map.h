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
		static Map* getSingleton();
        ~Map(){}
        void update(){}
        void load(std::string);
        void unload();
		vb01::Vector3 getCellPos();
		int getCellId();
		inline vb01::Node* getNodeParent(){return nodeParent;}
		inline vb01::Vector2 getSize(){return size;}
    private:
		vb01::Node *nodeParent = nullptr;
		std::string mapName;
		float cellSize = 1;
		GraphNode*** cells = nullptr;
		int **weights = nullptr;
		vb01::Vector2 size;

        Map(){}
		void loadSkybox(gameBase::LuaManager*);
		void loadTerrain(gameBase::LuaManager*);
		void loadWaterbodies(gameBase::LuaManager*);
		void generateCells();
    };
}

#endif
