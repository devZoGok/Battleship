#include "mapEditorAppState.h"
#include "gameManager.h"
#include "stateManager.h"
#include "guiAppState.h"
#include "defConfigs.h"
#include "cameraController.h"
#include "unitFrameController.h"
#include "player.h"
#include "map.h"
#include "mesh.h"
#include "util.h"

#include <box.h>
#include <node.h>
#include <quad.h>
#include <model.h>
#include <texture.h>
#include <assetManager.h>

#include <luaManager.h>

#include <listbox.h>

namespace battleship{
	using namespace configData;
	using namespace gameBase;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace std;

	MapEditorAppState::MapEditor::UnitListbox::UnitListbox(int startId, Vector2 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath){
		this->startId = startId;
	}

	void MapEditorAppState::MapEditor::UnitListbox::onClose(){
		UnitFrameController *ufCtr = UnitFrameController::getSingleton();
		
		int id = startId + selectedOption;
		LuaManager *lm = LuaManager::getSingleton();
		string modelPath = lm->getStringFromTable("basePath", vector<Index>{Index(id + 1)}) + lm->getStringFromTable("meshPath", vector<Index>{Index(id + 1)});
		ufCtr->addUnitFrame(UnitFrameController::UnitFrame(modelPath, id, (int)UnitType::LAND));

		ufCtr->setPlacingFrames(true);
	}

	MapEditorAppState::MapEditor::MapEditor(string name, Vector2 size){
		map = Map::getSingleton();
		map->load(name, true);

		map->addPlayer(new Player(0, 0, 0));

		generatePlane(size);
		prepareGui();

		string basePath = GameManager::getSingleton()->getPath();
		prepareTextures(basePath + "Textures/Skyboxes/", true, skyTextures);
		prepareTextures(basePath + "Textures/Landmass/", false, landmassTextures);

		AssetManager *assetManager = AssetManager::getSingleton();
		assetManager->load(basePath + "Models/Units/", true);
		assetManager->load(DEFAULT_TEXTURE);
	}

	//TODO fix crash when using 100 side verts
	void MapEditorAppState::MapEditor::generatePlane(Vector2 size){
		int numSideVerts = 10, numIndices = 6 * (numSideVerts - 1) * (numSideVerts - 1);
		MeshData::Vertex *vertices = new MeshData::Vertex[numSideVerts * numSideVerts];
		u32 *indices = new u32[numIndices];

		for(int i = 0; i < numSideVerts; i++){
			for(int j = 0; j < numSideVerts; j++){
				MeshData::Vertex vert;

				float x = size.x * (2 * j - numSideVerts) / (2 * numSideVerts);
				float z = size.y * (numSideVerts - 2 * i) / (2 * numSideVerts);

				vert.pos = Vector3(x, 0, z);
				vert.uv = Vector2((float)(j) / numSideVerts, (float)(i) / numSideVerts);
				vert.tan = Vector3::VEC_I;
				vert.biTan = Vector3::VEC_K;
				vert.norm = vert.tan.cross(vert.biTan);

				vertices[i * numSideVerts + j] = vert;
			}
		}

		for(int i = 0; i < numSideVerts - 1; i++){
			for(int j = 0; j < numSideVerts - 1; j++){
				int id = (i * (numSideVerts - 1) + j) * 6;
				indices[id] = i * numSideVerts + j;
				indices[id + 1] = i * numSideVerts + j + 1;
				indices[id + 2] = (i + 1) * numSideVerts + j + 1;

				indices[id + 3] = (i + 1) * numSideVerts + j + 1;
				indices[id + 4] = (i + 1) * numSideVerts + j;
				indices[id + 5] = i * numSideVerts + j;
			}
		}

		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() +  "texture");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addBoolUniform("lightingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4(1, 0, 0, 1));

		MeshData meshData(vertices, indices, numIndices / 3);
		Mesh *mesh = new Mesh(meshData);
		mesh->construct();
		mesh->setMaterial(mat);

		Node *par = new Node(), *node = new Node();
		node->attachMesh(mesh);
		par->attachChild(node);
		root->getRootNode()->attachChild(par);

		map->addTerrainObject(TerrainObject(Vector3(0, 0, 0), Vector3(size.x, 0, size.y), Vector3(14, 6, 14), TerrainObject::LANDMASS, par, 0, new Cell, new vb01::u32*));
	}

	void MapEditorAppState::MapEditor::prepareGui(){
		GameManager *gm = GameManager::getSingleton();
		StateManager *sm = gm->getStateManager();
		GuiAppState *state = (GuiAppState*)sm->getAppStateByType((int)AppStateType::GUI_STATE);
		string fontPath = gm->getPath() + "Fonts/batang.ttf";

		Vector2 startPos = Vector2((float)gm->getWidth() / 16, (float)gm->getHeight() * .8);
		Vector2 size = Vector2(140, 20), offset = Vector2(size.x + 10, 0);
		int maxDisplay = 2;

		vector<string> vehicles, structures;

		LuaManager *lm = LuaManager::getSingleton();
		int numUnits = lm->getInt("numUnits");

		for(int i = 0; i < numUnits; i++){
			bool vehicle = lm->getBoolFromTable("isVehicle", vector<Index>{Index(i + 1)});
			string name = lm->getStringFromTable("name", vector<Index>{Index(i + 1)});
			(vehicle ? vehicles : structures).push_back(name);
		}

		vehicleListbox = new UnitListbox(0, startPos, size, vehicles, maxDisplay, fontPath);
		state->addListbox(vehicleListbox);

		structureListbox = new UnitListbox(3, startPos + offset, size, structures, (structures.size() > maxDisplay ? maxDisplay : structures.size()), fontPath);
		state->addListbox(structureListbox);

		class SkyboxListbox : public Listbox{
			public:
				SkyboxListbox(Vector2 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath){}
				
				void onClose(){
					Root *root = Root::getSingleton();
					StateManager *sm = GameManager::getSingleton()->getStateManager();
					MapEditor *mapEditor = ((MapEditorAppState*)sm->getAppStateByType((int)AppStateType::MAP_EDITOR))->getMapEditor();
					Texture *skyTexture = mapEditor->getSkyTexture(selectedOption);
					
					if(!root->getSkybox())
						root->createSkybox(skyTexture);
					else
						 root->getSkybox()->getMaterial()->setTexUniform("tex", skyTexture, true);
				}
			private:
		};

		vector<string> skyboxFolders = readDir(gm->getPath() + "Textures/Skyboxes", true);
		SkyboxListbox *skyboxListbox = new SkyboxListbox(startPos + 2 * offset, size, skyboxFolders, (skyboxFolders.size() > maxDisplay ? maxDisplay : skyboxFolders.size()), fontPath);
		state->addListbox(skyboxListbox);

		class LandTexListbox : public Listbox{
			public:
				LandTexListbox(Vector2 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath){}

				void onClose(){
					StateManager *sm = GameManager::getSingleton()->getStateManager();
					MapEditor *mapEditor = ((MapEditorAppState*)sm->getAppStateByType((int)AppStateType::MAP_EDITOR))->getMapEditor();

					Map *map = Map::getSingleton();
					Material *mat = map->getTerrainObject(0).node->getChild(0)->getMesh(0)->getMaterial();
					Material::BoolUniform *texturingUniform = (Material::BoolUniform*)mat->getUniform("texturingEnabled");
					Texture *landTex = mapEditor->getLandmassTexture(selectedOption);
					string texUni = "textures[0]";

					if(!texturingUniform->value){
						texturingUniform->value = true;
						mat->addTexUniform(texUni, landTex, false);
					}
					else
						mat->setTexUniform(texUni, landTex, false);
				}
			private:
		};

		vector<string> landTextures = readDir(gm->getPath() + "Textures/Landmass", false);
		LandTexListbox *landTexListbox = new LandTexListbox(startPos + 3 * offset, size, landTextures, (landTextures.size() > maxDisplay ? maxDisplay : landTextures.size()), fontPath);
		state->addListbox(landTexListbox);
	}

	void MapEditorAppState::MapEditor::prepareTextures(string basePath, bool skybox, vector<Texture*> &textures){
		AssetManager *assetManager = AssetManager::getSingleton();
		assetManager->load(basePath, skybox);
		vector<string> texturePaths;

		for(int i = 0; i < assetManager->getNumAssets(); i++){
			string assetPath = assetManager->getAsset(i)->path;

			if(assetPath.length() >= basePath.length() && assetPath.substr(0, basePath.length()) == basePath)
				texturePaths.push_back(assetPath);
		}

		if(skybox){
			const int numTex = 6;

			for(int i = 0; i < texturePaths.size() / numTex; i++){
				string paths[]{
					texturePaths[i * numTex],
					texturePaths[i * numTex + 1],
					texturePaths[i * numTex + 2],
					texturePaths[i * numTex + 3],
					texturePaths[i * numTex + 4],
					texturePaths[i * numTex + 5],
				};

				textures.push_back(new Texture(paths, numTex, true));
			}
		}
		else{
			for(int i = 0; i < texturePaths.size(); i++){
				string paths[]{texturePaths[i]};
				textures.push_back(new Texture(paths, 1, false));
			}
		}
	}

	MapEditorAppState::MapEditorAppState(string name, Vector2 size) : AbstractAppState(
					AppStateType::MAP_EDITOR,
				 	configData::calcSumBinds(AppStateType::MAP_EDITOR, true),
				 	configData::calcSumBinds(AppStateType::MAP_EDITOR, false),
				 	GameManager::getSingleton()->getPath() + "Scripts/options.lua"){
		mapName = name;
		mapSize = size;
	}	

	void MapEditorAppState::update(){
		CameraController *camCtr = CameraController::getSingleton();

		if(!camCtr->isLookingAround())
			camCtr->updateCameraPosition();

		UnitFrameController *ufCtr = UnitFrameController::getSingleton();

		if(ufCtr->isPlacingFrames())
			ufCtr->update();
	}

	void MapEditorAppState::onAttached(){
		AbstractAppState::onAttached();
		mapEditor = new MapEditor(mapName, mapSize);
	}

	void MapEditorAppState::onDettached(){}

	void MapEditorAppState::onAction(int bind, bool isPressed){
		UnitFrameController *ufCtr = UnitFrameController::getSingleton();

		switch((Bind)bind){
			case Bind::LOOK_AROUND:
				if(ufCtr->isPlacingFrames()){
					Player *player = Map::getSingleton()->getPlayer(0);
					UnitFrameController::UnitFrame &frame = ufCtr->getUnitFrame(0);
					player->addUnit(new Unit(player, frame.id, frame.model->getPosition(), frame.model->getOrientation()));
				}
				else
					CameraController::getSingleton()->setLookingAround(isPressed);

                break;
			case Bind::DESELECT_STRUCTURE:
				ufCtr->removeUnitFrames();
				ufCtr->setPlacingFrames(false);
				break;
		}
	}

	void MapEditorAppState::onAnalog(int bind, float strength){
		CameraController *camCtr = CameraController::getSingleton();

		switch((Bind)bind){
			case Bind::LOOK_UP: 
			case Bind::LOOK_DOWN: 
				if(camCtr->isLookingAround()){
					Vector3 dirProj = Root::getSingleton()->getCamera()->getDirection();
					dirProj = Vector3(dirProj.x, 0, dirProj.z).norm();
					CameraController::getSingleton()->orientCamera(Vector3(0, 1, 0).cross(dirProj), strength);
				}

				break;
			case Bind::LOOK_LEFT: 
			case Bind::LOOK_RIGHT: 
				if(camCtr->isLookingAround())
					camCtr->orientCamera(Vector3(0, 1, 0), strength);

				break;
		}
	}
}
