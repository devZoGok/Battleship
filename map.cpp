#include <root.h>
#include <quad.h>
#include <model.h>
#include <material.h>
#include <assetManager.h>

#include <solUtil.h>

#include "map.h"
#include "player.h"
#include "unitFactory.h"
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

	void Map::update(){
		for(Player *p : players)
			p->update();
	}

	void Map::loadSkybox(){
		sol::state_view SOL_LUA_STATE = generateView();
		string skyboxName = SOL_LUA_STATE[mapTable]["skybox"];
		string basePath = GameManager::getSingleton()->getPath() + "Textures/Skyboxes/" + skyboxName;

		const int numPaths = 6;
		string path[numPaths] = {
			basePath + "/left.jpg",
			basePath + "/right.jpg",
			basePath + "/up.jpg",
			basePath + "/down.jpg",
			basePath + "/front.jpg",
			basePath + "/back.jpg"
		};

		for(int i = 0; i < numPaths; i++)
			AssetManager::getSingleton()->load(path[i]);

		Root::getSingleton()->createSkybox(path);
	}

	void Map::loadTerrainObject(int id){
		/*
		LuaManager *luaManager = LuaManager::getSingleton();
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
		string basePath = GameManager::getSingleton()->getPath();
		string texPath = basePath;

		//TODO remove this quick fix for landmass model loading
		if(id == -1){
			type = TerrainObject::LANDMASS;

			string mapPathSuffix = "Models/Maps/" + mapName + "/";
			texPath += mapPathSuffix;
			string terrainFile = basePath + mapPathSuffix + luaManager->getStringFromTable(mapTable, vector<Index>{terrInd, Index("model")});

			AssetManager::getSingleton()->load(terrainFile);
			node = (Model*)((new Model(terrainFile))->getChild(0));

			Node *par = node->getParent();
			par->dettachChild(node);
			delete par;
		}
		else{
			type = TerrainObject::RECT_WATERBODY;
			node = new Node();
			quad = new Quad(Vector3(size.x, size.z, 1), true);
			node->attachMesh(quad);

			texPath += "Textures/Water/";
		}

		nodeParent->attachChild(node);
		node->setPosition(pos);

		Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", false);

		ind = baseIndices;
		ind.push_back(Index("albedoMap"));
		texPath += luaManager->getStringFromTable(mapTable, ind);

		string fr[]{texPath};
		AssetManager::getSingleton()->load(fr[0]);
		Texture *t = new Texture(fr, 1, false);

		mat->addTexUniform("textures[0]", t, true);

		if(id == -1)
			((Model*)node)->setMaterial(mat);
		else
			quad->setMaterial(mat);

		terrainObjects.push_back(TerrainObject(pos, size, Vector3(cellSize.x, (id == -1 ? 0 : cellSize.y), cellSize.z), type, node, numCells, cells, weights));
		*/
	}

	void Map::loadSpawnPoints(){
		sol::state_view SOL_LUA_STATE = generateView();
		int numSpawnPoints = SOL_LUA_STATE[mapTable]["numSpawnPoints"];

		for(int i = 0; i < numSpawnPoints; i++){
			sol::table posTable = SOL_LUA_STATE[mapTable]["spawnPoints"][i + 1];
			spawnPoints.push_back(Vector3(posTable["x"], posTable["y"], posTable["z"]));
		}
	}

	//TODO simplify the dubplicatory field extraction statements
	void Map::loadPlayers(){
		AssetManager *assetManager = AssetManager::getSingleton();
		string path = GameManager::getSingleton()->getPath();
		assetManager->load(path + DEFAULT_TEXTURE);
		sol::state_view SOL_LUA_STATE = generateView();
		assetManager->load(path + (string)SOL_LUA_STATE["modelPrefix"], true);

		int numPlayers = SOL_LUA_STATE[mapTable]["numPlayers"];
		string playerInd = "players";

		for(int i = 0; i < numPlayers; i++){
			//int spawnPointId = SOL_LUA_STATE[mapTable]["spawnPointInd"][i + 1][spawnPointId];
			int numUnits = SOL_LUA_STATE[mapTable][playerInd][i + 1]["numUnits"];
			
			players.push_back(new Player(0, 0, 0, spawnPoints[i]));

			for(int j = 0; j < numUnits; j++){
				sol::table unitTable = SOL_LUA_STATE[mapTable][playerInd][i + 1]["units"][j + 1];

	   			string posInd = "pos";
				Vector3 pos = Vector3(unitTable[posInd]["x"], unitTable[posInd]["y"], unitTable[posInd]["z"]);

				string rotInd = "rot";
				Quaternion rot = Quaternion(unitTable[rotInd]["w"], unitTable[rotInd]["x"], unitTable[rotInd]["x"], unitTable[rotInd]["x"]);

				int id = unitTable["id"];
				
				players[i]->addUnit(UnitFactory::createUnit(players[i], id, pos, rot));
			}
		}
	}

	void Map::preprareScene(){
		terrainNode = new Node();
		Root *root = Root::getSingleton();
		root->getRootNode()->attachChild(terrainNode);

		Camera *cam = root->getCamera();
		cam->setPosition(Vector3(1, 1, 1) * 40);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
	}

    void Map::load(string mapName, bool empty) {
		this->mapName = mapName;

		Pathfinder::getSingleton()->setImpassibleNodeVal(u16(0 - 1));
		string path = GameManager::getSingleton()->getPath();
		SOL_LUA_STATE.script("PATH = \"" + path + "\"");
		SOL_LUA_STATE.script_file(path + configData::scripts[1]);
		
		preprareScene();

		if(!empty){
		sol::state_view SOL_LUA_STATE = generateView();
			SOL_LUA_STATE.script_file(GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + mapName + ".lua");
			int numWaterbodies = SOL_LUA_STATE[mapTable]["numWaterBodies"];
			
			loadSpawnPoints();
			loadPlayers();
			loadSkybox();
			loadTerrainObject(-1);
			
			for(int i = 0; i < numWaterbodies; i++)
				loadTerrainObject(i);
		}
    }

    void Map::unload() {}

	int Map::getCellId(Vector3 pos, int id){
		/*
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
		*/
		return 0;
	}
}
