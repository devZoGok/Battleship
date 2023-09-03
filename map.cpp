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
		string texPath = "";
		Quad *quad = nullptr;
		Node *node = nullptr;
		sol::state_view SOL_LUA_STATE = generateView();

		if(id == -1){
			string basePath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/";
			texPath = basePath + (string)SOL_LUA_STATE[mapTable]["terrain"]["albedo"];

			string terrainFile = basePath + (string)SOL_LUA_STATE[mapTable]["terrain"]["model"];
			AssetManager::getSingleton()->load(terrainFile);
			node = (Model*)((new Model(terrainFile))->getChild(0));

			Node *par = node->getParent();
			par->dettachChild(node);
			delete par;
		}
		else{
			sol::table waterBodyTable = SOL_LUA_STATE[mapTable]["waterbodies"][id + 1], posTable = waterBodyTable["pos"];
			texPath = GameManager::getSingleton()->getPath() + "Textures/Water/" + (string)waterBodyTable["albedo"];

			quad = new Quad(Vector3(waterBodyTable["size"]["x"], waterBodyTable["size"]["y"], 1), true);
			Vector3 pos = Vector3(posTable["x"], posTable["y"], posTable["z"]);
			node = new Node(pos);
			node->attachMesh(quad);
		}

		terrainNode->attachChild(node);

		Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", false);

		string fr[]{texPath};
		AssetManager::getSingleton()->load(fr[0]);
		Texture *t = new Texture(fr, 1, false);

		mat->addTexUniform("textures[0]", t, true);

		if(id == -1)
			((Model*)node)->setMaterial(mat);
		else
			quad->setMaterial(mat);
	}

	void Map::loadSpawnPoints(){
		sol::state_view SOL_LUA_STATE = generateView();
		int numSpawnPoints = SOL_LUA_STATE[mapTable]["numSpawnPoints"];

		for(int i = 0; i < numSpawnPoints; i++){
			sol::table posTable = SOL_LUA_STATE[mapTable]["spawnPoints"][i + 1];
			spawnPoints.push_back(Vector3(posTable["x"], posTable["y"], posTable["z"]));
		}
	}

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
		Root *root = Root::getSingleton();
		Node *rootNode = root->getRootNode();
		string libPath = root->getLibPath();
		terrainNode = new Node();
		rootNode->attachChild(terrainNode);

		cellNode = new Node();
		cellNode->setVisible(false);
		rootNode->attachChild(cellNode);
		landCellMat = new Material(libPath + "texture");
		landCellMat->addBoolUniform("lightingEnabled", false);
		landCellMat->addBoolUniform("texturingEnabled", false);
		landCellMat->addVec4Uniform("diffuseColor", Vector4(0, 1, 0, 1));
		waterCellMat = new Material(libPath + "texture");
		waterCellMat->addBoolUniform("lightingEnabled", false);
		waterCellMat->addBoolUniform("texturingEnabled", false);
		waterCellMat->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 1));

		Camera *cam = root->getCamera();
		cam->setPosition(Vector3(1, 1, 1) * 40);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
	}

	void Map::loadCells(){
		sol::state_view SOL_LUA_STATE = generateView();
		int numCells = SOL_LUA_STATE[mapTable]["numCells"];
		sol::table cellsTable = SOL_LUA_STATE[mapTable]["cells"];

		for(int i = 0; i < numCells; i++){
			sol::table cellTable = cellsTable[i + 1], posTable = cellTable["pos"];
			int numEdges = cellTable["numEdges"];
			vector<Edge> edges;

			for(int j = 0; j < numEdges; j++){
				sol::table edgeTable = cellTable["edges"][j + 1];
				edges.push_back(Edge(edgeTable["weight"], edgeTable["srcCellId"], edgeTable["destCellId"]));
			}

			Vector3 cellPos = Vector3(posTable["x"], posTable["y"], posTable["z"]);
			Cell::Type cellType = (Cell::Type)cellTable["type"];

			Quad *quad = new Quad(Vector3(CELL_SIZE.x, CELL_SIZE.z, 0));
			quad->setWireframe(true);
			quad->setMaterial(cellType == Cell::Type::LAND ? landCellMat : waterCellMat);

			Node *node = new Node(cellPos + Vector3::VEC_J * .1);
			node->attachMesh(quad);
			cellNode->attachChild(node);

			cells.push_back(Cell(cellPos, cellType, edges));
		}
	}

    void Map::load(string mapName, bool empty) {
		this->mapName = mapName;

		Pathfinder::getSingleton()->setImpassibleNodeVal(u16(0 - 1));
		string path = GameManager::getSingleton()->getPath();
		sol::state_view SOL_LUA_STATE = generateView();
		
		preprareScene();

		if(!empty){
			sol::state_view SOL_LUA_STATE = generateView();
			SOL_LUA_STATE.script_file(GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + mapName + ".lua");
			int numWaterbodies = SOL_LUA_STATE[mapTable]["numWaterBodies"];
			
			loadSpawnPoints();
			loadPlayers();
			loadSkybox();
			loadCells();
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
