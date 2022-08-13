#include <root.h>
#include <quad.h>
#include <model.h>
#include <material.h>
#include <assetManager.h>

#include <luaManager.h>

#include "map.h"
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

			Node *waterNode = new Node();
			waterNode->attachMesh(quad);
			root->getRootNode()->attachChild(waterNode);
			float posX = luaManager->getFloatFromTable(table, vector<Index>{Index("posX", true)});
			float posY = luaManager->getFloatFromTable(table, vector<Index>{Index("posY", true)});
			float posZ = luaManager->getFloatFromTable(table, vector<Index>{Index("posZ", true)});
			waterNode->setPosition(Vector3(posX, posY, posZ));
			waterNode->setOrientation(Quaternion(-1.57, Vector3::VEC_I));
			nodeParent->attachChild(waterNode);
		}
	}

    void Map::load(string mapName) {
		this->mapName = mapName;
		LuaManager *luaManager = LuaManager::getSingleton();
		luaManager->buildScript(vector<string>{GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + mapName + ".lua"});

		nodeParent = new Node();
		Root *root = Root::getSingleton();
		root->getRootNode()->attachChild(nodeParent);

		loadSkybox(luaManager);
		loadTerrain(luaManager);
		loadWaterbodies(luaManager);

		Camera *cam = root->getCamera();
		cam->setPosition(Vector3(1, 1, 1) * 10);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
    }

		void Map::generateCells(){
				//TODO replace magic values by retrieving terrain dimensions
				Vector3 terrainSize = Vector3(20, 6, 20);
				Vector3 initPos = terrainSize * (-0.5);
				int numCellsByDim[]{terrainSize.x / cellSize, terrainSize.y / cellSize, terrainSize.z / cellSize};
				cells = new GraphNode**[numCellsByDim[0]];

				for(int i = 0; i < numCellsByDim[0]; i++){
					cells[i] = new GraphNode*[numCellsByDim[1]];

					for(int j = 0; j < numCellsByDim[1]; j++)
						cells[i][j] = new GraphNode[numCellsByDim[2]];
				}

				for(int i = 0; i < numCellsByDim[0]; i++){
					for(int j = 0; j < numCellsByDim[1]; j++){
						for(int k = 0; k < numCellsByDim[2]; k++){
								GraphNode::Type type = GraphNode::SEA;

								GraphNode node;
								node.type = type;
								node.pos = initPos + Vector3(cellSize * i, cellSize * j, cellSize * k);
								cells[i][j][k] = node;
						}
					}
				}

				int numCells = numCellsByDim[0] * numCellsByDim[1] * numCellsByDim[2];
				weights = new int*[numCells];

				for(int i = 0; i < numCells; i++){
						weights[i] = new int[numCells];
						int x1 = i / numCellsByDim[2] / numCellsByDim[1];
						int y1 = i / numCellsByDim[2] % numCellsByDim[1];
						int z1 = i % numCellsByDim[2];

						for(int j = 0; j < numCells; j++){
								int x2 = j / numCellsByDim[2] / numCellsByDim[1];
								int y2 = j / numCellsByDim[2] % numCellsByDim[1];
								int z2 = j % numCellsByDim[2];
								int weight;

								if(abs(x1 - x2) == 1 || abs(y1 - y2) == 1 || abs(z1 - z2) == 1)
										weight = 1;
								else if(x1 == x2 && y1 == y2 && z1 == z2)
										weight = 0;
								else
										weight = -1;

								weights[i][j] = weight;
						}
				}
		}

    void Map::unload() {}

	Vector3 Map::getCellPos(){
		return Vector3::VEC_ZERO;
	}

	int Map::getCellId(){
		return 0;
	}
}
