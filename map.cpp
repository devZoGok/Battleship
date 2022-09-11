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

	bool WaterBody::isPointWithin(Vector3 p){
		bool within;

		if(rect)
			within = (fabs(pos.x - p.x) < size.x && fabs(pos.z - p.z) < size.y);
		else
			within = Vector2(pos.x, pos.z).getDistanceFrom(Vector2(p.x, p.z)) < size.x;

		return within;
	}

	Map *map = nullptr;

	Map* Map::getSingleton(){
		if(!map)
			map = new Map;

		return map;
	}

	void Map::loadSkybox(LuaManager *luaManager){
		int numPaths = 6;
		string table = "skybox", basePath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/";
		string path[] = {
			basePath + luaManager->getStringFromTable(table, vector<Index>{Index("left", true)}),
			basePath + luaManager->getStringFromTable(table, vector<Index>{Index("right", true)}),
			basePath + luaManager->getStringFromTable(table, vector<Index>{Index("up", true)}),
			basePath + luaManager->getStringFromTable(table, vector<Index>{Index("down", true)}),
			basePath + luaManager->getStringFromTable(table, vector<Index>{Index("front", true)}),
			basePath + luaManager->getStringFromTable(table, vector<Index>{Index("back", true)})
		};

		for(int i = 0; i < numPaths; i++)
			AssetManager::getSingleton()->load(path[i]);

		Root::getSingleton()->createSkybox(path);
	}

	void Map::loadTerrain(LuaManager *luaManager){
		string terrainFile = luaManager->getString("model");
		string albedoFile = luaManager->getString("albedoMap");

		AssetManager *assetManager = AssetManager::getSingleton();
		string basePath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/";
		assetManager->load(basePath + terrainFile);
		assetManager->load(basePath + albedoFile);

		Root *root = Root::getSingleton();
		Model *model = new Model(basePath + terrainFile);
		Material *mat = new Material(root->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", false);

		string fr[]{basePath + albedoFile};
		Texture *t = new Texture(fr, 1, false);

		mat->addTexUniform("textures[0]", t, true);
		model->setMaterial(mat);
		root->getRootNode()->attachChild(model);
		nodeParent->attachChild(model);
		terrainModel = model;
	}

	void Map::loadWaterbodies(LuaManager *luaManager){
		//TODO : replace the magic value
		int numWaterBodies = 1;
		string table = "waterBodies";
		Root *root = Root::getSingleton();

		for(int i = 0; i < numWaterBodies; i++){
			float sizeX = luaManager->getFloatFromTable(table, vector<Index>{Index("sizeX", true)});
			float sizeY = luaManager->getFloatFromTable(table, vector<Index>{Index("sizeY", true)});
			Quad *quad = new Quad(Vector3(sizeX, sizeY, 1) * 1, true);
			Material *mat = new Material(root->getLibPath() + "texture");
			mat->addBoolUniform("texturingEnabled", true);
			mat->addBoolUniform("lightingEnabled", false);

			string fr[]{GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + luaManager->getStringFromTable(table, vector<Index>{Index("albedoMap", true)})};
			AssetManager::getSingleton()->load(fr[0]);
			Texture *t = new Texture(fr, 1, false);

			mat->addTexUniform("textures[0]", t, true);
			quad->setMaterial(mat);

			bool rect = luaManager->getBoolFromTable(table, vector<Index>{Index("rect", true)});
			Node *waterNode = new Node();
			waterNode->attachMesh(quad);
			root->getRootNode()->attachChild(waterNode);
			float posX = luaManager->getFloatFromTable(table, vector<Index>{Index("posX", true)});
			float posY = luaManager->getFloatFromTable(table, vector<Index>{Index("posY", true)});
			float posZ = luaManager->getFloatFromTable(table, vector<Index>{Index("posZ", true)});
			waterNode->setPosition(Vector3(posX, posY, posZ));
			waterNode->setOrientation(Quaternion(-1.57, Vector3::VEC_I));
			nodeParent->attachChild(waterNode);

			WaterBody waterBody;
			waterBody.pos = Vector3(posX, posY, posZ);
			waterBody.size = Vector2(sizeX, sizeY);
			waterBody.rect = rect;
			waterBodies.push_back(waterBody);
		}
	}

    void Map::load(string mapName) {
		this->mapName = mapName;
		LuaManager *luaManager = LuaManager::getSingleton();
		luaManager->buildScript(vector<string>{GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + mapName + ".lua"});

		Pathfinder::getSingleton()->setImpassibleNodeVal(u16(0 - 1));

		nodeParent = new Node();
		Root *root = Root::getSingleton();
		root->getRootNode()->attachChild(nodeParent);

		loadSkybox(luaManager);
		loadTerrain(luaManager);
		loadWaterbodies(luaManager);

		Camera *cam = root->getCamera();
		cam->setPosition(Vector3(1, 1, 1) * 40);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
    }

    void Map::unload() {}

	Vector3 Map::getCellPos(int id, Vector3 cellSize){
		int numCellsX = size.x / cellSize.x;
		int numCellsZ = size.z / cellSize.z;

		int x = id % numCellsX;
		int y = id / (numCellsX * numCellsZ);
		int z = (id / numCellsX) % numCellsZ;

		Vector3 initPos = Vector3(size.x, 0, size.z) * -0.5 + Vector3(cellSize.x, 0, cellSize.z) * 0.5;
		return initPos + Vector3(x * cellSize.x, y * cellSize.y, z * cellSize.z);
	}

	int Map::getCellId(Vector3 pos, Vector3 cellSize){
		int numCellsX = size.x / cellSize.x;
		int numCellsZ = size.z / cellSize.z;

		Vector3 initPos = Vector3(size.x, 0, size.z) * -0.5;
		int x = fabs(pos.x - initPos.x) / cellSize.x;
		int y = (cellSize.y > 0 ? (fabs(pos.y - initPos.y) / cellSize.y) : 0);
		int z = fabs(pos.z - initPos.z) / cellSize.z;

		return (numCellsX * numCellsZ * y + (numCellsX * z + x));
	}

	bool Map::isPointWithin(int id, Vector3 point, Vector3 cellSize){
		Vector3 cellPos = getCellPos(id, cellSize);
		return ((fabs(point.x - cellPos.x) < 0.5 * cellSize.x) && (fabs(point.z - cellPos.z) < 0.5 * cellSize.z));
	}
}
