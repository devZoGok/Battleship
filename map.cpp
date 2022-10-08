#include <root.h>
#include <quad.h>
#include <model.h>
#include <material.h>
#include <assetManager.h>

#include <luaManager.h>

#include "map.h"
#include "pathfinder.h"
#include "gameManager.h"
#include "defConfigs.h"

using namespace std;
using namespace vb01;
using namespace gameBase;

namespace battleship{
	using namespace configData;

	Map *map = nullptr;

	Map* Map::getSingleton(){
		if(!map)
			map = new Map;

		return map;
	}

	void Map::loadSkybox(LuaManager *luaManager){
		int numPaths = 6;
		string skyTable = "skybox", basePath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/";
		string path[] = {
			basePath + luaManager->getStringFromTable(mapTable, vector<Index>{Index(skyTable), Index("left")}),
			basePath + luaManager->getStringFromTable(mapTable, vector<Index>{Index(skyTable), Index("right")}),
			basePath + luaManager->getStringFromTable(mapTable, vector<Index>{Index(skyTable), Index("up")}),
			basePath + luaManager->getStringFromTable(mapTable, vector<Index>{Index(skyTable), Index("down")}),
			basePath + luaManager->getStringFromTable(mapTable, vector<Index>{Index(skyTable), Index("front")}),
			basePath + luaManager->getStringFromTable(mapTable, vector<Index>{Index(skyTable), Index("back")})
		};

		for(int i = 0; i < numPaths; i++)
			AssetManager::getSingleton()->load(path[i]);

		Root::getSingleton()->createSkybox(path);
	}

	void Map::loadTerrainObject(LuaManager *luaManager, int id){
		string terrainTable = (id == -1 ? "terrain" : "waterBodies");
		vector<Index> baseIndices = (id == -1 ? vector<Index>{Index(terrainTable)} : vector<Index>{Index(terrainTable), Index(id + 1)});
		Vector3 pos = Vector3::VEC_ZERO, size = Vector3::VEC_ZERO;
		Index terrInd = Index("terrain"),
			  waterInd = Index("waterBodies"),
			  idIndex = Index(id + 1),
			  posInd = Index("pos"),
			  sizeInd = Index("size"),
			  xInd = Index("x"),
			  yInd = Index("y"),
			  zInd = Index("z"),
			  nodeInd = Index("nodes");

		vector<Index> sizeBaseInd;

		if(id == -1){
			sizeBaseInd.push_back(terrInd);
		}
		else{
			float x = luaManager->getFloatFromTable(mapTable, vector<Index>{waterInd, idIndex, posInd, xInd});
			float y = luaManager->getFloatFromTable(mapTable, vector<Index>{waterInd, idIndex, posInd, yInd});
			float z = luaManager->getFloatFromTable(mapTable, vector<Index>{waterInd, idIndex, posInd, zInd});
			pos = Vector3(x, y, z);

			sizeBaseInd = vector<Index>{waterInd, idIndex};
		}

		sizeBaseInd.push_back(sizeInd);
		vector<Index> indVecX = sizeBaseInd;
		vector<Index> indVecY = sizeBaseInd;
		vector<Index> indVecZ = sizeBaseInd;
		indVecX.push_back(xInd);
		indVecY.push_back(yInd);
		indVecZ.push_back(zInd);
		float x = luaManager->getFloatFromTable(mapTable, indVecX);
		float y = luaManager->getFloatFromTable(mapTable, indVecY);
		float z = luaManager->getFloatFromTable(mapTable, indVecZ);
		size = Vector3(x, y, z);

		vector<Index> baseNodeInd = baseIndices;
		baseNodeInd.push_back(nodeInd);

		vector<Index> ind = baseNodeInd;
		ind.push_back(Index("numCells"));
		int numCells = luaManager->getIntFromTable(mapTable, ind);

		u32 **weights = new u32*[numCells];
		Cell *cells = new Cell[numCells];

		for(int i = 0; i < numCells; i++){
			weights[i] = new u32[numCells];

			for(int j = 0; j < numCells; j++){
				int wid = i * numCells + j;
				ind = baseNodeInd;
				ind.push_back(Index("weights"));
				ind.push_back(Index(wid + 1));
				u32 w = luaManager->getIntFromTable(mapTable, ind);
				weights[i][j] = w;
			}

			ind = baseNodeInd;
			ind.push_back(Index("impassible"));
			ind.push_back(Index(i + 1));
			bool impassible = luaManager->getFloatFromTable(mapTable, ind);

			vector<Index> posBaseInd = baseNodeInd;
			posBaseInd.push_back(posInd);
			posBaseInd.push_back(Index(i + 1));

			vector<Index> indVecX = posBaseInd;
			vector<Index> indVecY = posBaseInd;
			vector<Index> indVecZ = posBaseInd;
			indVecX.push_back(xInd);
			indVecY.push_back(yInd);
			indVecZ.push_back(zInd);
			float x = luaManager->getFloatFromTable(mapTable, indVecX);
			float y = luaManager->getFloatFromTable(mapTable, indVecY);
			float z = luaManager->getFloatFromTable(mapTable, indVecZ);
			cells[i] = Cell(Vector3(x, y, z), (id == -1), impassible);
		}

		Node *node = nullptr;
		Quad *quad = nullptr;
		TerrainObject::Type type;
		string basePath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/";

		if(id == -1){
			type = TerrainObject::LANDMASS;

			string terrainFile = basePath + luaManager->getStringFromTable(mapTable, vector<Index>{terrInd, Index("model")});
			AssetManager::getSingleton()->load(terrainFile);
			node = new Model(terrainFile);
		}
		else{
			type = TerrainObject::RECT_WATERBODY;
			node = new Node();
			quad = new Quad(Vector3(size.x, size.z, 1), true);
			node->attachMesh(quad);
			node->setOrientation(Quaternion(-1.57, Vector3::VEC_I));
		}

		nodeParent->attachChild(node);
		node->setPosition(pos);

		Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", false);

		ind = baseIndices;
		ind.push_back(Index("albedoMap"));
		string fr[]{basePath + luaManager->getStringFromTable(mapTable, ind)};
		AssetManager::getSingleton()->load(fr[0]);
		Texture *t = new Texture(fr, 1, false);

		mat->addTexUniform("textures[0]", t, true);

		if(id == -1)
			((Model*)node)->setMaterial(mat);
		else
			quad->setMaterial(mat);

		terrainObjects.push_back(TerrainObject(pos, size, Vector3(14, (id == -1 ? 0 : 4), 14), type, node, numCells, cells, weights));
	}

	/*
	void Map::loadWaterbodies(LuaManager *luaManager){
		int numWaterBodies = luaManager->getIntFromTable(mapTable, vector<Index>{Index("numWaterBodies")});
		string table = "waterBodies";
		Root *root = Root::getSingleton();

		for(int i = 1; i <= numWaterBodies; i++){
			float sizeX = luaManager->getFloatFromTable(mapTable, vector<Index>{Index(table), Index(i), Index("sizeX")});
			float sizeY = luaManager->getFloatFromTable(mapTable, vector<Index>{Index(table), Index(i), Index("sizeY")});
			Quad *quad = new Quad(Vector3(sizeX, sizeY, 1) * 1, true);
			Material *mat = new Material(root->getLibPath() + "texture");
			mat->addBoolUniform("texturingEnabled", false);
			mat->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 0.5));
			mat->addBoolUniform("lightingEnabled", false);

			string fr[]{GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + luaManager->getStringFromTable(mapTable, vector<Index>{Index(table), Index(i), Index("albedoMap")})};
			AssetManager::getSingleton()->load(fr[0]);
			Texture *t = new Texture(fr, 1, false);

			mat->addTexUniform("textures[0]", t, true);
			quad->setMaterial(mat);

			bool rect = luaManager->getBoolFromTable(mapTable, vector<Index>{Index(table), Index("rect")});
			Node *waterNode = new Node();
			waterNode->attachMesh(quad);
			root->getRootNode()->attachChild(waterNode);
			float posX = luaManager->getFloatFromTable(mapTable, vector<Index>{Index(table), Index(i), Index("posX")});
			float posY = luaManager->getFloatFromTable(mapTable, vector<Index>{Index(table), Index(i), Index("posY")});
			float posZ = luaManager->getFloatFromTable(mapTable, vector<Index>{Index(table), Index(i), Index("posZ")});
			waterNode->setPosition(Vector3(posX, posY, posZ));
			waterNode->setOrientation(Quaternion(-1.57, Vector3::VEC_I));
			nodeParent->attachChild(waterNode);

			Vector3 pos = Vector3(posX, posY, posZ), size = Vector3(sizeX, 0, sizeY);
			terrainObjects.push_back(TerrainObject(pos, size, TerrainObject::RECT_WATERBODY, waterNode));
		}
	}
	*/

    void Map::load(string mapName) {
		this->mapName = mapName;
		LuaManager *luaManager = LuaManager::getSingleton();
		luaManager->buildScript(vector<string>{GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + mapName + ".lua"});

		Pathfinder::getSingleton()->setImpassibleNodeVal(u16(0 - 1));

		int numWaterbodies = luaManager->getIntFromTable(mapTable, vector<Index>{Index("numWaterBodies")});
		nodeParent = new Node();
		Root *root = Root::getSingleton();
		root->getRootNode()->attachChild(nodeParent);

		loadSkybox(luaManager);
		loadTerrainObject(luaManager, -1);

		for(int i = 0; i < numWaterbodies; i++)
			loadTerrainObject(luaManager, i);

		Camera *cam = root->getCamera();
		cam->setPosition(Vector3(1, 1, 1) * 40);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
    }

    void Map::unload() {}

	int Map::getCellId(Vector3 pos, int id){
		Vector3 regionSize = terrainObjects[id].size;
		Vector3 regionPos = terrainObjects[id].pos;
		Vector3 cellSize = terrainObjects[id].cellSize;

		int numCellsX = regionSize.x / cellSize.x;
		int numCellsZ = regionSize.z / cellSize.z;

		Vector3 initPos = regionPos - Vector3(regionSize.x, 0, regionSize.z) * 0.5;
		int x = fabs(pos.x - initPos.x) / cellSize.x;
		int y = (cellSize.y > 0 ? (fabs(pos.y - initPos.y) / cellSize.y) : 0);
		int z = fabs(pos.z - initPos.z) / cellSize.z;

		return (numCellsX * numCellsZ * y + (numCellsX * z + x));
	}

	bool Map::isPointWithinTerrainObject(Vector3 p, int id){
		bool within;
		Vector3 pos = terrainObjects[id].pos, size = terrainObjects[id].size;

		if(terrainObjects[id].type == TerrainObject::RECT_WATERBODY)
			within = (fabs(pos.x - p.x) < size.x && fabs(pos.z - p.z) < size.y);
		else if(terrainObjects[id].type == TerrainObject::ROUND_WATERBODY)
			within = Vector2(pos.x, pos.z).getDistanceFrom(Vector2(p.x, p.z)) < size.x;
		else
			within = true;

		return within;
	}

	/*
	Vector3 Map::getCellPos(int id, Vector3 cellSize, int waterbodyId){
		Vector3 regionSize = size;
		Vector3 regionPos = Vector3::VEC_ZERO;

		if(waterbodyId != -1){
			Vector3 s = terrainObjects[waterbodyId].size;
			regionSize = Vector3(s.x, 0, s.y);
			regionPos = terrainObjects[waterbodyId].pos;
		}

		int numCellsX = regionSize.x / cellSize.x;
		int numCellsZ = regionSize.z / cellSize.z;

		int x = id % numCellsX;
		int y = id / (numCellsX * numCellsZ);
		int z = (id / numCellsX) % numCellsZ;

		Vector3 initPos = regionPos - (Vector3(regionSize.x, 0, regionSize.z) - Vector3(cellSize.x, 0, cellSize.z)) * .5;
		return initPos + Vector3(x * cellSize.x, -y * cellSize.y, z * cellSize.z);
	}


	bool Map::isPointWithin(int cellId, int waterbodyId, Vector3 point, Vector3 cellSize, bool cellSpatial){
		Vector3 cellPos = getCellPos(cellId, cellSize, waterbodyId);
		bool withinX = (fabs(point.x - cellPos.x) < 0.5 * cellSize.x);
		bool withinY = (cellSpatial ? (fabs(point.y - cellPos.y) < 0.5 * cellSize.y) : true);
		bool withinZ = (fabs(point.z - cellPos.z) < 0.5 * cellSize.z);
		return withinX && withinY && withinZ;
	}
	*/
}
