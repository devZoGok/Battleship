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
	class Player;

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
        void update();
        void load(std::string, bool = false);
        void unload();
		int getCellId(vb01::Vector3, int);
		bool isPointWithinTerrainObject(vb01::Vector3, int);
		void addTerrainObject(TerrainObject);
		inline std::string getMapName(){return mapName;}
		inline TerrainObject& getTerrainObject(int i){return terrainObjects[i];}
		inline int getNumTerrainObjects(){return terrainObjects.size();}
		inline vb01::Node* getNodeParent(){return nodeParent;}
		inline vb01::Vector3 getCellSize(){return cellSize;}
        inline std::vector<Player*> getPlayers() {return players;}
		inline Player* getPlayer(int i){return players[i];}
		inline void addPlayer(Player *p){players.push_back(p);}
		inline int getNumPlayers(){return players.size();}
		inline vb01::Vector3 getSpawnPoint(int i){return spawnPoints[i];}
		inline void addSpawnPoint(vb01::Vector3 sp){spawnPoints.push_back(sp);}
    private:
		std::string mapTable = "map";
		vb01::Node *nodeParent = nullptr;
		std::string mapName;
		std::vector<TerrainObject> terrainObjects;
		vb01::Vector3 cellSize;
		std::vector<vb01::Vector3> spawnPoints;
        std::vector<Player*> players;

        Map(){}
		void preprareScene();
		void loadSpawnPoints();
		void loadPlayers();
		void loadSkybox();
		void loadTerrainObject(int);
    };
}

#endif
