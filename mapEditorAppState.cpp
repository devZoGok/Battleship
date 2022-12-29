#include "mapEditorAppState.h"
#include "gameManager.h"
#include "defConfigs.h"
#include "cameraController.h"
#include "map.h"
#include "mesh.h"
#include "util.h"

#include <node.h>

namespace battleship{
	using namespace configData;
	using namespace vb01;
	using namespace gameBase;
	using namespace std;

	MapEditorAppState::MapEditor::MapEditor(string name, Vector2 size){
		map = Map::getSingleton();
		map->load(name, true);

		generatePlane(size);
	}

	void MapEditorAppState::MapEditor::generatePlane(Vector2 size){
		int numSideVerts = 100, numIndices = 6 * (numSideVerts - 1) * (numSideVerts - 1);
		MeshData::Vertex *vertices = new MeshData::Vertex[numSideVerts * numSideVerts];
		u32 *indices = new u32[numIndices];

		for(int i = 0; i < numSideVerts; i++){
			for(int j = 0; j < numSideVerts; j++){
				MeshData::Vertex vert;

				float x = size.x * (2 * j - numSideVerts) / (2 * numSideVerts);
				float z = size.y * (numSideVerts - 2 * i) / (2 * numSideVerts);

				vert.pos = Vector3(x, 0, z);
				vert.uv = Vector2((float)(j / numSideVerts), (float)(i / numSideVerts));
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

		MeshData meshData(vertices, indices, numIndices / 6);
		Mesh *mesh = new Mesh(meshData);
		mesh->construct();
		mesh->setMaterial(mat);

		Node *node = new Node();
		node->attachMesh(mesh);
		root->getRootNode()->attachChild(node);

		map->addTerrainObject(TerrainObject(Vector3(0, 0, 0), Vector3(size.x, 0, size.y), Vector3(14, 6, 14), TerrainObject::LANDMASS, node, 0, new Cell, new vb01::u32*));
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
	}

	void MapEditorAppState::onAttached(){
		AbstractAppState::onAttached();
		mapEditor = new MapEditor(mapName, mapSize);
	}

	void MapEditorAppState::onDettached(){}

	void MapEditorAppState::onAction(int bind, bool isPressed){
		switch((Bind)bind){
			case Bind::LOOK_AROUND:
				CameraController::getSingleton()->setLookingAround(isPressed);
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
