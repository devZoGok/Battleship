#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>

#include <vector.h>

namespace gameBase{
	class LuaManager;
}

namespace battleship{
	struct WaterBody{
		vb01::Vector3 pos;
		vb01::Vector2 size;
		bool rect;
	};

    class Map {
    public:
		static Map* getSingleton();
        ~Map(){}
        void update(){}
        void load(std::string);
        void unload();
		vb01::Vector3 getCellPos(int);
		int getCellId(vb01::Vector3);
		inline WaterBody getWaterBody(int i){return waterBodies[i];}
		inline vb01::Model* getTerrainModel(){return terrainModel;}
		inline vb01::Node* getNodeParent(){return nodeParent;}
		inline vb01::Vector3 getSize(){return size;}
    private:
		vb01::Node *nodeParent = nullptr;
		vb01::Model *terrainModel = nullptr;
		std::string mapName;
		WaterBody *waterBodies = nullptr;
		vb01::Vector3 size = vb01::Vector3(20, 20, 6);

        Map(){}
		void loadSkybox(gameBase::LuaManager*);
		void loadTerrain(gameBase::LuaManager*);
		void loadWaterbodies(gameBase::LuaManager*);
    };
}

#endif
