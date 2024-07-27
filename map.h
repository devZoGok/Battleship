#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

#include <rayCaster.h>
#include <vector.h>
#include <util.h>

namespace vb01{
	class Model;
	class Material;
	class Node;
}

namespace battleship{
	class Player;
	class Unit;
	class ResourceDeposit;
	struct Cell;

    class Map {
    public:
		struct Cell;
		
		struct Edge{
			Edge(vb01::s64 w, vb01::s64 src, vb01::s64 dest) : weight(w), srcCellId(src), destCellId(dest){}
			vb01::s64 weight, srcCellId, destCellId;
		};
		
		struct Cell{
			enum Type{LAND, WATER};
		
			Type type;
			vb01::Vector3 pos;
			std::vector<Edge> edges;
			std::vector<int> underWaterCellIds;
		
			Cell(){}
			Cell(vb01::Vector3 p, Type t, std::vector<Edge> e = std::vector<Edge>{}, std::vector<int> uc = std::vector<int>{}): pos(p), type(t), edges(e), underWaterCellIds(uc){}
		};

		static Map* getSingleton();
        ~Map(){}
		static std::vector<Edge> generateAdjacentNodeEdges(int, int, int, int, int);
        void update();
        void load(std::string, bool = false);
        void unload();
		std::vector<vb01::RayCaster::CollisionResult> raycastTerrain(vb01::Vector3, vb01::Vector3, bool);
		int getCellId(vb01::Vector3, bool = true);
		bool isPointWithinTerrainObject(vb01::Vector3, int);
		void loadPlayerGameObjects();
		inline std::string getMapName(){return mapName;}
		inline vb01::Node* getNodeParent(){return terrainNode;}
		inline vb01::Vector3 getCellSize(){return CELL_SIZE;}
		inline int getNumSpawnPoints(){return spawnPoints.size();}
		inline vb01::Vector3 getSpawnPoint(int i){return spawnPoints[i];}
		inline void addSpawnPoint(vb01::Vector3 sp){spawnPoints.push_back(sp);}
		inline std::vector<Map::Cell>& getCells(){return cells;}
    private:
		std::string mapTable = "map";
		vb01::Node *terrainNode = nullptr, *cellNode = nullptr;
		vb01::Material *landCellMat = nullptr, *waterCellMat = nullptr;
		std::string mapName;
		vb01::Vector3 CELL_SIZE = vb01::Vector3(7, 7, 7), mapSize;
		std::vector<vb01::Vector3> spawnPoints;
		std::vector<Cell> cells;

        Map(){}
		void preprareScene();
		void loadSpawnPoints();
		void loadSkybox();
		void loadCells();
		void loadTerrainObject(int);
		void unloadTerrainObjects();
		void unloadCells();
		void unloadSkybox();
		void unloadPlayerObjects();
		void destroyScene();
		template<typename T> int bsearch(std::vector<T>, T, float);
    };
}

#endif
