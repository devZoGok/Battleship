#include <root.h>
#include <quad.h>
#include <model.h>
#include <light.h>
#include <material.h>
#include <assetManager.h>

#include <solUtil.h>

#include "map.h"
#include "game.h"
#include "player.h"
#include "gameObject.h"
#include "gameObjectFactory.h"
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

	vector<Map::Edge> Map::generateAdjacentNodeEdges(int numVertCells, int i, int numHorCells, int j, int weight){
		vector<Map::Edge> edges;
		bool up = (i > 0), right = (j < numHorCells - 1), down = (i < numVertCells - 1), left = (j > 0);

		if(left)
			edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * i + j - 1));

		if(right)
			edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * i + j + 1));

		if(up)
			edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * (i - 1) + j));

		if(down)
			edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * (i + 1) + j));

		if(up && left)
			edges.push_back(Map::Edge(int(sqrt(2 * weight * weight)), numVertCells * i + j, numVertCells * (i - 1) + j - 1));

		if(up && right)
			edges.push_back(Map::Edge((sqrt(2 * weight * weight)), numVertCells * i + j, numVertCells * (i - 1) + j + 1));

		if(down && left)
			edges.push_back(Map::Edge((sqrt(2 * weight * weight)), numVertCells * i + j, numVertCells * (i + 1) + j - 1));

		if(down && right)
			edges.push_back(Map::Edge((sqrt(2 * weight * weight)), numVertCells * i + j, numVertCells * (i + 1) + j + 1));

		return edges;
	}

	void Map::update(){
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

	void Map::loadLights(){
		sol::state_view SOL_LUA_VIEW = generateView();
		sol::table lightsTbl = SOL_LUA_VIEW[mapTable]["lights"];
		int numLights = lightsTbl.size();

		for(int i = 0; i < numLights; i++){
			int id = i + 1;
			Light::Type type = (Light::Type)lightsTbl[id]["type"];
			sol::table colTbl = lightsTbl[id]["color"];

			Light *light = new Light(type);
			light->setColor(Vector3(colTbl["x"], colTbl["y"], colTbl["z"]));

			Node *lightNode = new Node();
			lights.push_back(lightNode);
			lightNode->addLight(light);

			if(type == Light::Type::DIRECTIONAL){
				sol::table dirTbl = lightsTbl[id]["dir"];
				Vector3 dir = Vector3(dirTbl["x"], dirTbl["y"], dirTbl["z"]).norm();
				lightNode->lookAt(dir);
			}

			Root::getSingleton()->getRootNode()->attachChild(lightNode);
		}
	}

	void Map::loadTerrainObject(int id){
		string texPath = "", albedoPath = "";
		Quad *quad = nullptr;
		Node *node = nullptr;
		sol::state_view SOL_LUA_STATE = generateView();

		if(id == -1){
			string basePath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/"; 

			albedoPath = SOL_LUA_STATE[mapTable]["terrain"]["albedo"];
			texPath = basePath + albedoPath;

			string modelPath = SOL_LUA_STATE[mapTable]["terrain"]["model"]; 
			string terrainFile = basePath + modelPath;

			AssetManager::getSingleton()->load(terrainFile);
			node = (Model*)((new Model(terrainFile))->getChild(0));

			Node *par = node->getParent();
			par->dettachChild(node);
			delete par;
		}
		else{
			sol::table waterBodyTable = SOL_LUA_STATE[mapTable]["waterbodies"][id + 1], posTable = waterBodyTable["pos"];
			albedoPath = waterBodyTable["albedo"];
			texPath = GameManager::getSingleton()->getPath() + "Textures/Water/" + albedoPath;

			quad = new Quad(Vector3(waterBodyTable["size"]["x"], waterBodyTable["size"]["y"], 1), true);
			Vector3 pos = Vector3(posTable["x"], posTable["y"], posTable["z"]);
			node = new Node(pos);
			node->attachMesh(quad);
		}

		terrainNode->attachChild(node);

		Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", true);
		mat->addBoolUniform("constLightingEnabled", true);
		mat->addBoolUniform("normalMapEnabled", false);
		mat->addBoolUniform("specularMapEnabled", false);
		mat->addBoolUniform("castShadow", false);

		string fr[]{texPath};
		AssetManager::getSingleton()->load(fr[0]);
		Texture *t = new Texture(fr, 1, false);

		mat->addTexUniform("textures[0]", t, true);

		if(id == -1)
			((Model*)node)->setMaterial(mat);
		else{
			mat->setTransparent(true);
			quad->setMaterial(mat);
		}
	}

	void Map::loadSpawnPoints(){
		sol::state_view SOL_LUA_VIEW = generateView();
		string spawnPointInd = "spawnPoints";
		sol::table spawnPointsTbl = SOL_LUA_VIEW[mapTable][spawnPointInd];
		int numSpawnPoints = spawnPointsTbl.size();

		for(int i = 0; i < numSpawnPoints; i++){
			sol::table posTable = SOL_LUA_VIEW[mapTable][spawnPointInd][i + 1];
			spawnPoints.push_back(Vector3(posTable["x"], posTable["y"], posTable["z"]));
		}
	}

	void Map::loadPlayerGameObjects(){
		AssetManager *assetManager = AssetManager::getSingleton();
		string path = GameManager::getSingleton()->getPath();
		assetManager->load(path + DEFAULT_TEXTURE);

		sol::state_view SOL_LUA_VIEW = generateView();
		string vfxPrefix = SOL_LUA_VIEW["vfxPrefix"], gameObjPrefix = SOL_LUA_VIEW["gameObjPrefix"];
		assetManager->load(path + vfxPrefix, true);
		assetManager->load(path + gameObjPrefix, true);
		assetManager->load(path + "Textures/", true);

		string playerInd = "players";
		SOL_LUA_VIEW.script("numPlayers = #" + mapTable + "." + playerInd);
		int numPlayers = SOL_LUA_VIEW["numPlayers"];

		for(int i = 0; i < numPlayers; i++){
			string resDepInd = "resourceDeposits";
			SOL_LUA_VIEW.script("numResourceDeposits = #" + mapTable + "." + playerInd + "[" + to_string(i + 1) + "]." + resDepInd);
			int numNpcObjs = SOL_LUA_VIEW["numResourceDeposits"];
			Player *player = Game::getSingleton()->getPlayer(i);

			for(int j = 0; j < numNpcObjs; j++){
				sol::table npcObjTable = SOL_LUA_VIEW[mapTable][playerInd][i + 1][resDepInd][j + 1];
				int id = npcObjTable["id"];

				sol::table posTable = npcObjTable["pos"];
				Vector3 pos = Vector3(posTable["x"], posTable["y"], posTable["z"]);

				sol::table rotTable = npcObjTable["rot"];
				Quaternion rot = Quaternion(rotTable["w"], rotTable["x"], rotTable["y"], rotTable["z"]);
				int initAmmount = SOL_LUA_VIEW[mapTable][playerInd][i + 1][resDepInd][j + 1]["initAmmount"];

				player->addResourceDeposit(GameObjectFactory::createResourceDeposit(player, id, pos, rot, initAmmount));
			}

			//int spawnPointId = SOL_LUA_STATE[mapTable]["spawnPointInd"][i + 1][spawnPointId];
			string unitInd = "units";
			SOL_LUA_VIEW.script("numUnits = #" + mapTable + "." + playerInd + "[" + to_string(i + 1) + "]." + unitInd);
			int numUnits = SOL_LUA_VIEW["numUnits"];
			
			for(int j = 0; j < numUnits; j++){
				sol::table unitTable = SOL_LUA_VIEW[mapTable][playerInd][i + 1][unitInd][j + 1];

	   			string posInd = "pos";
				Vector3 pos = Vector3(unitTable[posInd]["x"], unitTable[posInd]["y"], unitTable[posInd]["z"]);

				string rotInd = "rot";
				Quaternion rot = Quaternion(unitTable[rotInd]["w"], unitTable[rotInd]["x"], unitTable[rotInd]["x"], unitTable[rotInd]["x"]);

				int id = unitTable["id"];
				int buildStatus = unitTable["buildStatus"].get_or(0);
				player->addUnit(GameObjectFactory::createUnit(player, id, pos, rot, buildStatus));
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
		cam->setFarPlane(600);
		cam->setPosition(Vector3(1, 1, 1) * configData::CAMERA_DISTANCE);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());

	}

	//TODO implement toggleable cell rendering
	void Map::loadCells(){
		sol::state_view SOL_LUA_VIEW = generateView();
		int numCells = SOL_LUA_VIEW[mapTable]["numCells"];
		sol::table cellsTable = SOL_LUA_VIEW[mapTable]["cells"];

		for(int i = 0; i < numCells; i++){
			sol::table cellTable = cellsTable[i + 1], posTable = cellTable["pos"];
			int numEdges = cellTable["numEdges"];
			vector<Edge> edges;

			for(int j = 0; j < numEdges; j++){
				sol::table edgeTable = cellTable["edges"][j + 1];
				edges.push_back(Edge(edgeTable["weight"], edgeTable["srcCellId"], edgeTable["destCellId"]));
			}

			int numUnderWaterCells = cellTable["numUnderWaterCells"];
			vector<int> underWaterCellIds;

			for(int j = 0; j < numUnderWaterCells; j++)
				underWaterCellIds.push_back((int)cellTable["underWaterCellId"][j + 1]);

			Vector3 cellPos = Vector3(posTable["x"], posTable["y"], posTable["z"]);
			Cell::Type cellType = (Cell::Type)cellTable["type"];

			/*
			Quad *quad = new Quad(Vector3(CELL_SIZE.x, CELL_SIZE.z, 0));
			quad->setWireframe(true);
			quad->setMaterial(cellType == Cell::Type::LAND ? landCellMat : waterCellMat);

			Node *node = new Node(cellPos + Vector3::VEC_J * .1);
			node->attachMesh(quad);
			 */

			cells.push_back(Cell(cellPos, cellType, edges, underWaterCellIds));
		}
	}

	//TODO implement map size calculation when exporting maps
    void Map::load(string mapName, bool empty) {
		this->mapName = mapName;

		Pathfinder::getSingleton()->setImpassibleNodeVal(u16(0 - 1));
		string path = GameManager::getSingleton()->getPath();
		sol::state_view SOL_LUA_STATE = generateView();
		
		preprareScene();

		if(!empty){
			SOL_LUA_STATE.script_file(path + "Models/Maps/" + mapName + "/" + mapName + ".lua");
			sol::optional<sol::table> lightsOpt = SOL_LUA_STATE[mapTable]["lights"];
			
			if(lightsOpt != sol::nullopt)
				loadLights();

			loadSpawnPoints();
			loadSkybox();
			loadCells();
			loadTerrainObject(-1);

			sol::table sizeTable = SOL_LUA_STATE[mapTable]["size"];
			mapSize = Vector3(sizeTable["x"], sizeTable["y"], sizeTable["z"]);
			int numWaterbodies = SOL_LUA_STATE[mapTable]["numWaterBodies"];
			
			for(int i = 0; i < numWaterbodies; i++)
				loadTerrainObject(i);
		}
		else{
			Light *light = new Light(Light::Type::AMBIENT);
			light->setColor(Vector3::VEC_IJK * .9);

			Node *node = new Node();
			node->addLight(light);
			Root::getSingleton()->getRootNode()->attachChild(node);
			lights.push_back(node);
		}
    }

	//TODO unload skybox assets
	void Map::unloadSkybox(){
		Root::getSingleton()->removeSkybox();
	}

	void Map::unloadLights(){
		while(!lights.empty()){
			Root::getSingleton()->getRootNode()->dettachChild(lights[0]);
			delete lights[0];
			lights.erase(lights.begin());
		}
	}

	void Map::unloadCells(){
		while(cellNode->getNumChildren() > 0){
			Node *node = cellNode->getChild(0);
			node->getMesh(0)->setMaterial(nullptr);
			cellNode->dettachChild(node);
			delete node;
		}

		delete landCellMat;
		delete waterCellMat;

		cells.clear();
	}

	//TODO unload terrain assets
	void Map::unloadTerrainObjects(){
		while(terrainNode->getNumChildren() > 0){
			Node *node = terrainNode->getChild(0);
			terrainNode->dettachChild(node);
			delete node;
		}
	}

	void Map::unloadPlayerObjects(){
		for(Player *pl : Game::getSingleton()->getPlayers()){
			while(pl->getNumUnits() > 0){
				pl->removeUnit(0);
			}

			while(pl->getNumResourceDeposits() > 0){
				pl->removeResourceDeposit(0);
			}
		}
	}

	void Map::destroyScene(){
		Node *rootNode = Root::getSingleton()->getRootNode();
		rootNode->dettachChild(terrainNode);
		rootNode->dettachChild(cellNode);

		delete terrainNode;
		delete cellNode;
	}

    void Map::unload(){
		unloadSkybox();
		unloadLights();
		unloadCells();
		unloadTerrainObjects();
		unloadPlayerObjects();
		spawnPoints.clear();
		destroyScene();
	}

	vector<RayCaster::CollisionResult> Map::raycastTerrain(Vector3 rayPos, Vector3 rayDir, bool bothTerrTypes){
		vector<RayCaster::CollisionResult> allResults = RayCaster::cast(rayPos, rayDir, terrainNode->getChild(0), 0, configData::DIST_FROM_RAY);

		if(bothTerrTypes){
			vector<Node*> waterNodes = terrainNode->getChildren();
			waterNodes.erase(waterNodes.begin());

			vector<RayCaster::CollisionResult> waterResults = RayCaster::cast(rayPos, rayDir, waterNodes);
			allResults.insert(allResults.end(), waterResults.begin(), waterResults.end());

			RayCaster::sortResults(allResults);
		}

		return allResults;
	}

	template<typename T> int Map::bsearch(vector<T> haystack, T needle, float eps){
		T haystackMidVal;
		bool sizeHaystackEven = (haystack.size() % 2 == 0);
		int midValId = haystack.size() / 2;

		if(sizeHaystackEven)
			haystackMidVal = (haystack[midValId - 1] + haystack[midValId]) / 2;
		else
			haystackMidVal = haystack[midValId];

		int beginId, endId;

		if(fabs(haystackMidVal - needle) > eps){
			if(fabs(haystackMidVal - needle) < eps){
				beginId = 0;
				endId = (midValId - 1);
			}
			else{
				endId = haystack.size() - 1;
				endId = (midValId + 1);
			}
			
			return bsearch(vector<T>(haystack.begin() + beginId, haystack.begin() + endId), needle);
		}
		else{
			if(sizeHaystackEven)
				return midValId - (needle > haystackMidVal ? 0 : 1);
			else
				return midValId;
		}
	}

	//TODO replace search with binary search
	int Map::getCellId(Vector3 pos, bool checkUnderwaterCells){
		int numHorCells = int(mapSize.x / CELL_SIZE.x), horId = -1;

		for(int i = 0; i < numHorCells; i++)
			if(fabs(cells[i].pos.x - pos.x) < .5 * CELL_SIZE.x){
				horId = i;
				break;
			}

		int numVertCells = int(mapSize.z / CELL_SIZE.z), vertId = -1;

		for(int i = 0; i < numVertCells; i++)
			if(fabs(cells[i * numHorCells].pos.z - pos.z) < .5 * CELL_SIZE.z){
				vertId = i;
				break;
			}

		int surfaceCellId = vertId * numHorCells + horId;

		if(checkUnderwaterCells && cells[surfaceCellId].type == Cell::Type::WATER && !cells[surfaceCellId].underWaterCellIds.empty()){
			int cellId = surfaceCellId;

			for(int i = 0; i <= cells[surfaceCellId].underWaterCellIds.size(); i++){
				cellId = (i == 0 ? surfaceCellId : cells[surfaceCellId].underWaterCellIds[i - 1]);

				if(fabs(cells[cellId].pos.y - pos.y) < .5 * CELL_SIZE.y)
					return cellId;
			}
		}
		else
			return surfaceCellId;
	}
}
