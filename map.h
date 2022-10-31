#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

#include <vector.h>
#include <util.h>

namespace gameBase{
	class LuaManager;
}

namespace vb01{
	class Model;
	class Node;
}

namespace battleship{
	struct Cell{
		bool land, impassible;
		vb01::Vector3 pos;

		Cell(){}
		Cell(vb01::Vector3 p, bool l, bool i): pos(p), land(l), impassible(i){}
	};

	struct TerrainObject{
		enum Type{LANDMASS, RECT_WATERBODY, ROUND_WATERBODY};
		vb01::Vector3 pos, size, cellSize;
		Type type;
		vb01::Node *node = nullptr;
		int numCells;
		Cell *cells = nullptr;
		vb01::u32 **weights = nullptr;

		TerrainObject(vb01::Vector3 p, vb01::Vector3 s, vb01::Vector3 cs, Type t, vb01::Node *n, int num, Cell *c, vb01::u32 **w) :
		   	pos(p),
		   	size(s),
		   	cellSize(cs),
		   	type(t),
		   	node(n),
		   	numCells(num),
		   	cells(c),
		   	weights(w){}
	};

    class Map {
    public:
		static Map* getSingleton();
        ~Map(){}
        void update(){}
        void load(std::string);
        void unload();
		int getCellId(vb01::Vector3, int);
		bool isPointWithinTerrainObject(vb01::Vector3, int);
		inline TerrainObject getTerrainObject(int i){return terrainObjects[i];}
		inline int getNumTerrainObjects(){return terrainObjects.size();}
		inline vb01::Node* getNodeParent(){return nodeParent;}
		inline vb01::Vector3 getCellSize(){return cellSize;}
    private:
		std::string mapTable = "map";
		vb01::Node *nodeParent = nullptr;
		std::string mapName;
		std::vector<TerrainObject> terrainObjects;
		vb01::Vector3 cellSize;

        Map(){}
		void preprareScene();
		void loadSkybox(gameBase::LuaManager*);
		void loadTerrainObject(gameBase::LuaManager*, int);
    };
}

#endif
