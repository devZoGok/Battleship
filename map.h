#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

#include <vector.h>

namespace gameBase{
	class LuaManager;
}

namespace vb01{
	class Model;
	class Node;
}

namespace battleship{
	struct WaterBody{
		vb01::Vector3 pos;
		vb01::Vector2 size;
		bool rect;

		bool isPointWithin(vb01::Vector3);
	};

    class Map {
    public:
		static Map* getSingleton();
        ~Map(){}
        void update(){}
        void load(std::string);
        void unload();
		vb01::Vector3 getCellPos(int, vb01::Vector3, int);
		int getCellId(vb01::Vector3, vb01::Vector3, int);
		bool isPointWithin(int, int, vb01::Vector3, vb01::Vector3, bool = false);
		inline WaterBody getWaterBody(int i){return waterBodies[i];}
		inline int getNumWaterBodies(){return waterBodies.size();}
		inline vb01::Model* getTerrainModel(){return terrainModel;}
		inline vb01::Node* getNodeParent(){return nodeParent;}
		inline vb01::Vector3 getSize(){return size;}
    private:
		std::string terrTable = "terrain";
		vb01::Node *nodeParent = nullptr;
		vb01::Model *terrainModel = nullptr;
		std::string mapName;
		std::vector<WaterBody> waterBodies;
		vb01::Vector3 size;

        Map(){}
		void loadSkybox(gameBase::LuaManager*);
		void loadTerrain(gameBase::LuaManager*);
		void loadWaterbodies(gameBase::LuaManager*);
    };
}

#endif
