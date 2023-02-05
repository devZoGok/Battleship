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

#include <ray.h>
#include <box.h>
#include <node.h>
#include <quad.h>
#include <model.h>
#include <texture.h>
#include <assetManager.h>

#include <luaManager.h>

#include <listbox.h>

#include <fstream>

#include <tinyxml2.h>

namespace battleship{
	using namespace tinyxml2;
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

	MapEditorAppState::MapEditor::MapEditor(string name, Vector2 size, bool newMap){
		this->newMap = newMap;
		this->mapSize = size;

		map = Map::getSingleton();
		map->load(name, newMap);

		if(newMap){
			map->addPlayer(new Player(0, 0, 0));
			map->addSpawnPoint(Vector3::VEC_ZERO);
			generatePlane(size);
		}

		prepareGui();

		string basePath = GameManager::getSingleton()->getPath();
		prepareTextures(basePath + "Textures/Skyboxes/", true, skyTextures);
		prepareTextures(basePath + "Textures/Landmass/", false, landmassTextures);
		prepareTextures(basePath + "Textures/Water/", false, waterTextures);

		AssetManager *assetManager = AssetManager::getSingleton();
		assetManager->load(basePath + "Models/Units/", true);
		assetManager->load(DEFAULT_TEXTURE);
	}

	void MapEditorAppState::MapEditor::updateCircleRadius(bool increase){
		circleRadius += (increase ? INCREASE_RATE : -INCREASE_RATE);

		if(circleRadius > MAX_RADIUS)
			circleRadius = MAX_RADIUS;
		else if(circleRadius < MIN_RADIUS)
			circleRadius = MIN_RADIUS;
	}

	void MapEditorAppState::MapEditor::toggleSelection(TerrainObject *obj, bool select){
		obj->node->getMesh(0)->setWireframe(select);
		selectedTerrainObject = (select ? obj : nullptr);
	}

	void MapEditorAppState::MapEditor::castSelectionRay(){
		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 startPos = cam->getPosition();
		Vector3 endPos = screenToSpace(getCursorPos());

		vector<Ray::CollisionResult> results;
		Map *map = Map::getSingleton();
		Ray::retrieveCollisions(startPos, (endPos - startPos).norm(), map->getNodeParent(), results);
		Ray::sortResults(results);

		if(selectedTerrainObject)
			toggleSelection(selectedTerrainObject, false);

		if(!results.empty()){
			for(int i = 0; i < map->getNumTerrainObjects(); i++)
				if(map->getTerrainObject(i).node == results[i].mesh->getNode()){
					toggleSelection(&map->getTerrainObject(i), true);
					break;
				}
		}
	}


	void MapEditorAppState::MapEditor::createWaterbody(){
		Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
		mat->addBoolUniform("lightingEnabled", false);
		mat->addBoolUniform("texturingEnabled", true);
		mat->addTexUniform("textures[0]", waterTextures[0], false);

		Vector3 size = Vector3(10, 10, 1);
		Quad *quad = new Quad(size, true);
		quad->setMaterial(mat);

		Vector3 pos = 0.1 * Vector3::VEC_J;
		Node *node = new Node(pos, Quaternion(-1.57, Vector3::VEC_I));
		node->attachMesh(quad);

		Map *map = Map::getSingleton();
		TerrainObject::Type type = TerrainObject::RECT_WATERBODY;
		map->addTerrainObject(TerrainObject(pos, size, Vector3(14, 7, 14), type, node, 0, nullptr, nullptr));

		int objId = map->getNumTerrainObjects() - 1;
		TerrainObject &obj = map->getTerrainObject(objId); 
		toggleSelection(&obj, true);

		prepareTerrainObjects(objId - 1, 1);
		prepareCellMarkers(obj);
	}

	void MapEditorAppState::MapEditor::moveTerrainObject(float strength){
		Vector3 pos = selectedTerrainObject->node->getPosition();

		switch(movementAxis){
			case MovementAxis::X_AXIS:
				pos += strength * Vector3::VEC_I;
				break;
			case MovementAxis::Y_AXIS:
				pos += strength * Vector3::VEC_J;
				break;
			case MovementAxis::Z_AXIS:
				pos += strength * Vector3::VEC_K;
				break;
		}

		selectedTerrainObject->node->setPosition(pos);
		selectedTerrainObject->pos = pos;
	}

	void MapEditorAppState::MapEditor::pushLandmassVerts(float strength){
		Mesh *mesh = map->getTerrainObject(0).node->getMesh(0);
		MeshData meshData = mesh->getMeshBase();
		MeshData::Vertex *verts = meshData.vertices;
		int numVerts = meshData.numTris * 3;

		for(int i = 0; i < numVerts; i++){
			Vector3 distVec = verts[i].pos - pushPos;
			distVec.y = 0;
			float dist = distVec.getLength();

			if(dist < circleRadius)
				verts[i].pos.y += 10 * strength * dist / circleRadius;
		}

		mesh->updateVerts(meshData);
	}

	void MapEditorAppState::MapEditor::generatePlane(Vector2 size){
		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() +  "texture");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addBoolUniform("lightingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4(1, 0, 0, 1));

		Quad *mesh = new Quad(Vector3(size.x, size.y, 0), true, NUM_SUBDIVS, NUM_SUBDIVS);
		mesh->setMaterial(mat);

		Node *node = new Node();
		node->attachMesh(mesh);

		TerrainObject::Type type = TerrainObject::LANDMASS;
		Vector3 pos = Vector3::VEC_ZERO, s = Vector3(size.x, 0, size.y);
		map->addTerrainObject(TerrainObject(pos, s, Vector3(14, 6, 14), type, node, 0, nullptr, nullptr));

		prepareTerrainObjects(0, 1);
		prepareCellMarkers(map->getTerrainObject(0));
	}

	void MapEditorAppState::MapEditor::prepareGui(){
		GameManager *gm = GameManager::getSingleton();
		StateManager *sm = gm->getStateManager();
		GuiAppState *state = (GuiAppState*)sm->getAppStateByType((int)AppStateType::GUI_STATE);
		string fontPath = gm->getPath() + "Fonts/batang.ttf";

		Vector2 startPos = Vector2((float)gm->getWidth() / 16, gm->getHeight() - guiThreshold);
		Vector2 size = Vector2(140, 20), offset = Vector2(size.x + 10, 0);

		class ExportButton : public Button{
			public:
				ExportButton(Vector2 pos, Vector2 size) : Button(pos, size, "Export", GameManager::getSingleton()->getPath() + "Fonts/batang.ttf"){}
				void onClick(){
					StateManager *sm = GameManager::getSingleton()->getStateManager();
					MapEditor *mapEditor = ((MapEditorAppState*)sm->getAppStateByType((int)AppStateType::MAP_EDITOR))->getMapEditor();
					mapEditor->exportMap();
				}
		};

		state->addButton(new ExportButton(startPos, size));

		int maxDisplay = 2;

		vector<string> vehicles, structures;

		LuaManager *lm = LuaManager::getSingleton();
		int numUnits = lm->getInt("numUnits");

		for(int i = 0; i < numUnits; i++){
			bool vehicle = lm->getBoolFromTable("isVehicle", vector<Index>{Index(i + 1)});
			string name = lm->getStringFromTable("name", vector<Index>{Index(i + 1)});
			(vehicle ? vehicles : structures).push_back(name);
		}

		vehicleListbox = new UnitListbox(0, startPos + offset, size, vehicles, maxDisplay, fontPath);
		state->addListbox(vehicleListbox);

		structureListbox = new UnitListbox(3, startPos + 2 * offset, size, structures, (structures.size() > maxDisplay ? maxDisplay : structures.size()), fontPath);
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
		skyListbox = new SkyboxListbox(startPos + 3 * offset, size, skyboxFolders, (skyboxFolders.size() > maxDisplay ? maxDisplay : skyboxFolders.size()), fontPath);
		state->addListbox(skyListbox);

		class LandTexListbox : public Listbox{
			public:
				LandTexListbox(Vector2 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath){}

				void onClose(){
					StateManager *sm = GameManager::getSingleton()->getStateManager();
					MapEditor *mapEditor = ((MapEditorAppState*)sm->getAppStateByType((int)AppStateType::MAP_EDITOR))->getMapEditor();

					Map *map = Map::getSingleton();
					Material *mat = map->getTerrainObject(0).node->getMesh(0)->getMaterial();
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
		LandTexListbox *landTexListbox = new LandTexListbox(startPos + 4 * offset, size, landTextures, (landTextures.size() > maxDisplay ? maxDisplay : landTextures.size()), fontPath);
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

	void MapEditorAppState::MapEditor::parseLandmass(){
		XMLDocument *doc = new XMLDocument();

		char *nodeTagName = "node";
		XMLElement *rootEl = doc->NewElement(nodeTagName);
		rootEl->SetAttribute("name", "terrain");
		XMLNode *rootTag = doc->InsertEndChild(rootEl);

		XMLElement *nodeEl = doc->NewElement(nodeTagName);
		nodeEl->SetAttribute("name", "plane");
		XMLNode *nodeTag = rootTag->InsertEndChild(nodeEl);

		XMLElement *meshEl = doc->NewElement("mesh");
		MeshData meshData = map->getTerrainObject(0).node->getMesh(0)->getMeshBase();
		meshEl->SetAttribute("name", "mesh");
		meshEl->SetAttribute("num_faces", meshData.numTris);
		meshEl->SetAttribute("num_vertex_groups", 0);
		meshEl->SetAttribute("num_shape_keys", 0);
		XMLNode *meshTag = nodeTag->InsertEndChild(meshEl);

		//TODO optimize vertex position extraction
		/*
		const int numSideQuads = NUM_SUBDIVS + 1, numSideVerts = numSideQuads + 1;
		Vector2 subQuadSize = Vector2(size.x / numSideQuads, size.y / numSideQuads);
		*/
		int numVerts = 3 * meshData.numTris;

		for(int i = 0; i < numVerts; i++){
			XMLElement *vertEl = doc->NewElement("vertdata");
			vertEl->SetAttribute("px", meshData.vertices[i].pos.x);
			vertEl->SetAttribute("py", meshData.vertices[i].pos.y);
			vertEl->SetAttribute("pz", meshData.vertices[i].pos.z);
			vertEl->SetAttribute("nx", meshData.vertices[i].norm.x);
			vertEl->SetAttribute("ny", meshData.vertices[i].norm.y);
			vertEl->SetAttribute("nz", meshData.vertices[i].norm.z);
			XMLNode *vertNode = meshTag->InsertEndChild(vertEl);
		}

		for(int i = 0; i < numVerts; i++){
			XMLElement *vertEl = doc->NewElement("vert");
			vertEl->SetAttribute("id", i);
			vertEl->SetAttribute("uvx", meshData.vertices[i].uv.x);
			vertEl->SetAttribute("uvy", meshData.vertices[i].uv.y);
			vertEl->SetAttribute("tx", meshData.vertices[i].tan.x);
			vertEl->SetAttribute("ty", meshData.vertices[i].tan.y);
			vertEl->SetAttribute("tz", meshData.vertices[i].tan.z);
			vertEl->SetAttribute("bx", meshData.vertices[i].biTan.x);
			vertEl->SetAttribute("by", meshData.vertices[i].biTan.y);
			vertEl->SetAttribute("bz", meshData.vertices[i].biTan.z);
			XMLNode *vertTag = meshTag->InsertEndChild(vertEl);
		}

		string name = GameManager::getSingleton()->getPath() + "Models/Maps/" + map->getMapName() + "/" + map->getMapName() + ".xml";
		doc->SaveFile(name.c_str());
	}

	void MapEditorAppState::MapEditor::deleteWeights(){
		weightsGenerated = false;
	}

	void MapEditorAppState::MapEditor::prepareTerrainObject(u32 **weights, Cell *cells, int cellsByDim[3], float height, bool land){
		const int numCells = cellsByDim[0] * cellsByDim[1] * cellsByDim[2];
		Vector3 initPos = -.5 * Vector3(mapSize.x, -height, mapSize.y);

		Node *landmass = map->getTerrainObject(0).node;
		
		for(int i = 0; i < numCells; i++){
		    int xId = i % cellsByDim[0];
		    int yId = int(i / (cellsByDim[0] * cellsByDim[2]));
		    int zId = (int(i / cellsByDim[0])) % cellsByDim[2];

		    cells[i].pos = initPos + Vector3(xId * cellLength, -yId * cellDepth, zId * cellWidth);

			if(land){
				vector<Ray::CollisionResult> res;
				Ray::retrieveCollisions(Vector3(cells[i].pos.x, 100, cells[i].pos.z), -Vector3::VEC_J, landmass, res);
				Ray::sortResults(res);

				if(!res.empty())
					cells[i].pos.y = res[0].pos.y;
			}
		}

		MeshData meshData = landmass->getMesh(0)->getMeshBase();
		
		for(int i = 0; i < numCells; i++){
		    int waterbodyId = -1;
		    bool impassible = false;
			TerrainObject waterbody;
		
		    for(int j = 0; j < 3 * meshData.numTris; j++){
		        Vector3 point = Vector3(float(meshData.vertices[j].pos.x), float(meshData.vertices[j].pos.y), float(meshData.vertices[j].pos.z));
		
		        for(int k = 1; k < map->getNumTerrainObjects(); k++){
		            TerrainObject wb = map->getTerrainObject(k);
		            bool diffX = (abs(point.x - wb.pos.x) < .5 * wb.size.x);
		            bool diffY = (abs(point.y - wb.pos.z) < .5 * wb.size.z);
		            bool diffZ = (abs(point.z + wb.pos.y) < .5 * wb.size.y);
		            
		            if(diffX && diffZ){
		                waterbodyId = k;
		                break;
					}
				}
		
		        if(waterbodyId != -1)
		            waterbody = map->getTerrainObject(waterbodyId);
		
		        bool withinX = (abs(point.x - cells[i].pos.x) < 0.5 * cellLength);
		        bool withinY = (!land ? (abs(point.y - cells[i].pos.y) < 0.5 * cellDepth) : true);
		        bool withinZ = (abs(point.z - cells[i].pos.z) < 0.5 * cellWidth);
		        bool pointWithin = (withinX && withinY && withinZ);
		
		        if(land && pointWithin && (waterbodyId != -1 && point.y < waterbody.pos.z))
		            impassible = true;
				else if(!land && pointWithin && waterbodyId != -1)
		            impassible = true;
			}
		}
		
		for(int i = 0; i < numCells; i++){
		    int xId = i % cellsByDim[0];
		    int yId = i / (cellsByDim[0] * cellsByDim[2]);
		    int zId = (i / cellsByDim[0]) % cellsByDim[2];
		
		    for(int j = 0; j < numCells; j++){
		        bool adjacent = false;
				
		        if(xId == 0 && j - i == 1)
		            adjacent = true;
				else if(xId == cellsByDim[0] - 1 && j - i == -1)
		            adjacent = true;
				else if(0 < xId && xId < cellsByDim[0] - 1 && abs(j - i) == 1)
		            adjacent = true;
		        
		        if(yId == 0 && j - i == cellsByDim[0] * cellsByDim[2])
		            adjacent = true;
				else if(0 < yId && yId < cellsByDim[1] - 1 && abs(j - i) == cellsByDim[0] * cellsByDim[2])
		            adjacent = true;
				else if(yId == cellsByDim[1] - 1 && j - i == -(cellsByDim[0] * cellsByDim[2]))
		            adjacent = true;
		        
		        if(zId == 0 && j - i == cellsByDim[0])
		            adjacent = true;
				else if(zId == cellsByDim[2] - 1 && j - i == -cellsByDim[0])
		            adjacent = true;
				else if(0 < zId && zId < cellsByDim[2] - 1 && abs(j - i) == cellsByDim[0])
		            adjacent = true;
		
		        weights[i][j] = IMPASS_NODE_VAL;
		
		        if(i == j)
		            weights[i][j] = 0;
				else if(adjacent)
		            weights[i][j] = 1;
			}
		}
	}

	void MapEditorAppState::MapEditor::prepareTerrainObjects(int startId, int numObjs){
		if(numObjs == -1)
			numObjs = map->getNumTerrainObjects();

		for(int i = startId; i < numObjs; i++){
			TerrainObject &obj = map->getTerrainObject(i);
			Vector3 size = obj.size;
			int cellsByDim[]{
				int(size.x / cellLength),
				i == 0 ? 1 : int(size.y / cellDepth),
				int(size.z / cellWidth)
			};
			const int numCells = cellsByDim[0] * cellsByDim[1] * cellsByDim[2];

			Cell *cells = new Cell[numCells];

			u32 **weights = new u32*[numCells];

			for(int i = 0; i < numCells; i++)
				weights[i] = new u32[numCells];

			obj.cells = cells;
			obj.numCells = numCells;
			obj.weights = weights;

			prepareTerrainObject(weights, cells, cellsByDim, (i > 0 ? obj.pos.y : 0), i == 0);
		}
	}

	string MapEditorAppState::MapEditor::parseTerrainObject(TerrainObject &terrObj){
		string terrObjStr = "{\n";

		if(terrObj.type = TerrainObject::LANDMASS)
			terrObjStr += "model = \"" + map->getMapName() + ".xml\",\n";

		terrObjStr += "albedoMap = \"" + map->getMapName() + ".jpg\",\n";
		terrObjStr += "size = {x = " + to_string(terrObj.pos.x) + ", y = " + to_string(terrObj.pos.y) + ", z = " + to_string(terrObj.pos.z) + "},\n";
		terrObjStr += "nodes = {\n";
		terrObjStr += "numCells = " + to_string(terrObj.numCells) + ",\n";
		terrObjStr += "size = {x = " + to_string(cellLength) + ", y = " + to_string(cellDepth) + ", z = " + to_string(cellWidth) + "},\n";
		terrObjStr += "pos = {\n";

		for(int i = 0; i < terrObj.numCells; i++)
			terrObjStr += "{x = " + to_string(terrObj.cells[i].pos.x) + ", y = " + to_string(terrObj.cells[i].pos.y) + ", z = " + to_string(terrObj.cells[i].pos.z) + "},\n";

		terrObjStr += "},\nimpassible = {\n";

		for(int i = 0; i < terrObj.numCells; i++){
			terrObjStr += (terrObj.cells[i].impassible ? "true" : "false");
			terrObjStr += ",\n";
		}

		terrObjStr += "},\nweights = {\n";

		for(int i = 0; i < terrObj.numCells; i++)
			for(int j = 0; j < terrObj.numCells; j++)
				terrObjStr += to_string(terrObj.weights[i][j]) + ", ";

		terrObjStr += "}\n}\n},\n";

		return terrObjStr;
	}

	void MapEditorAppState::MapEditor::parseMapScript(){
		int numTerrObjs = map->getNumTerrainObjects();
		string mapScript = "map = {\nnumWaterBodies = " + to_string(numTerrObjs) + ",\n";
		mapScript += "impassibleNodeValue = " + to_string(IMPASS_NODE_VAL) + ",\n";
		mapScript += "numPlayers = " + to_string(map->getNumPlayers()) + ",\n";

		int numSpawnPoints = map->getNumSpawnPoints();
		mapScript += "numSpawnPoints = " + to_string(numSpawnPoints) + ",\n";
		mapScript += "spawnPoints = {\n";

		for(int i = 0; i < numSpawnPoints; i++){
			Vector3 sp = map->getSpawnPoint(i);
			mapScript += "{x = " + to_string(sp.x) + ", y = " + to_string(sp.y) + ", z = " + to_string(sp.z) + "},\n";
		}

		mapScript += "},\nplayers = {\n";

		for(int i = 0; i < map->getNumPlayers(); i++){
			mapScript += "spawnPoint = 0,\n";

			int numUnits = map->getPlayer(i)->getNumberOfUnits();
			mapScript += "numUnits = " + to_string(numUnits) + ",\n";

			if(numUnits > 0){
				mapScript += "units = {\n";

				for(int j = 0; j < numUnits; j++){
					Unit *unit = map->getPlayer(i)->getUnit(j);

					string idStr = "id = " + to_string(unit->getId());

					Vector3 unitPos = unit->getPos();
					string posStr = "pos = {x = " + to_string(unitPos.x) + ", y = " + to_string(unitPos.y) + ", z = " + to_string(unitPos.z) + "}";

					Quaternion unitRot = unit->getRot();
					string rotStr = "rot = {w = " + to_string(unitRot.w) + ", x = " + to_string(unitRot.x) + ", y = " + to_string(unitRot.y) + ", z = " + to_string(unitRot.z) + "}";

					mapScript += "{" + idStr + ", " + posStr + ", " + rotStr + "},\n";
				}

				mapScript += "}\n,";
				mapScript += "}";
			}
		}

		mapScript += "},\n";
		mapScript += "skybox = \"" + wstringToString(skyListbox->getContents()[skyListbox->getSelectedOption()]) + "\",\n";
		mapScript += "terrain = " + parseTerrainObject(map->getTerrainObject(0));
		mapScript += "waterbodies = {";

		for(int i = 1; i < numTerrObjs; i++)
			mapScript += parseTerrainObject(map->getTerrainObject(i));

		mapScript += "}\n}";

		string file = GameManager::getSingleton()->getPath() + "Models/Maps/" + map->getMapName() + "/" + map->getMapName() + ".lua";

		std::ofstream outFile(file);
		outFile << mapScript;
		outFile.close();
	}

	void MapEditorAppState::MapEditor::toggleCellMarkers(){
		cellMarkersVisible = !cellMarkersVisible;

		for(TerrainObject &obj : map->getTerrainObjects())
			for(Node *cellMarker : obj.cellMarkers)
				cellMarker->setVisible(cellMarkersVisible);
	}

	void MapEditorAppState::MapEditor::prepareCellMarkers(TerrainObject &obj){
		Root *root = Root::getSingleton();
		Node *rootNode = root->getRootNode();

		for(int i = 0; i < obj.numCells; i++){
			Material *mat = new Material(root->getLibPath() + "texture");
			mat->addBoolUniform("lightingEnabled", false);
			mat->addBoolUniform("texturingEnabled", false);
			mat->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 1));

			Box *box = new Box(Vector3(1, 3, 1));
			box->setMaterial(mat);

			Node *cellMarker = new Node(obj.cells[i].pos);
			cellMarker->attachMesh(box);
			cellMarker->setVisible(cellMarkersVisible);
			rootNode->attachChild(cellMarker);
			obj.cellMarkers.push_back(cellMarker);
		}
	}

	void MapEditorAppState::MapEditor::exportMap(){
		prepareTerrainObjects();
		parseLandmass();
		parseMapScript();
	}

	MapEditorAppState::MapEditorAppState(string name, Vector2 size, bool newMap) : AbstractAppState(
					AppStateType::MAP_EDITOR,
				 	configData::calcSumBinds(AppStateType::MAP_EDITOR, true),
				 	configData::calcSumBinds(AppStateType::MAP_EDITOR, false),
				 	GameManager::getSingleton()->getPath() + "Scripts/options.lua"){
		mapName = name;
		mapSize = size;
		this->newMap = newMap;
	}

	void MapEditorAppState::update(){
		radiusText->setText(L"Radius: " + to_wstring(mapEditor->getCircleRadius()));
		weightsText->setText(L"Weights generated: " + to_wstring(mapEditor->isWeightsGenerated()));

		CameraController *camCtr = CameraController::getSingleton();

		if(!(camCtr->isLookingAround() || mapEditor->isPushing()))
			camCtr->updateCameraPosition();

		UnitFrameController *ufCtr = UnitFrameController::getSingleton();

		if(ufCtr->isPlacingFrames())
			ufCtr->update();
	}

	void MapEditorAppState::onAttached(){
		AbstractAppState::onAttached();
		mapEditor = new MapEditor(mapName, mapSize, newMap);

		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() + "text");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4::VEC_IJKL);

		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/batang.ttf";
		radiusText = new Text(fontPath, L"");
		radiusText->setMaterial(mat);
		weightsText = new Text(fontPath, L"");
		weightsText->setMaterial(mat);

		Node *radiusNode = new Node(Vector3(0, 100, 0));
		radiusNode->addText(radiusText);
		root->getGuiNode()->attachChild(radiusNode);

		Node *weightsNode = new Node(Vector3(0, 200, 0));
		weightsNode->addText(weightsText);
		root->getGuiNode()->attachChild(weightsNode);
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
			case Bind::INCREASE_RADIUS:
				if(isPressed) mapEditor->updateCircleRadius(true);
				break;
			case Bind::DECREASE_RADIUS:
				if(isPressed) mapEditor->updateCircleRadius(false);
				break;
			case Bind::LEFT_CLICK:
				if(isPressed){
					Camera *cam = Root::getSingleton()->getCamera();
					Vector3 startPos = cam->getPosition();
					Vector2 cursorPos = getCursorPos();
					Vector3 endPos = screenToSpace(cursorPos);

					vector<Ray::CollisionResult> results;
					Ray::retrieveCollisions(startPos, (endPos - startPos).norm(), Map::getSingleton()->getTerrainObject(0).node, results);
					Ray::sortResults(results);


					if(cursorPos.y < GameManager::getSingleton()->getHeight() - mapEditor->getGuiThreshold()){
						bool push = !results.empty();
						mapEditor->setPushing(push);

						if(push)
							mapEditor->setPushPos(results[0].pos);

						mapEditor->castSelectionRay();
					}
					else
						mapEditor->setPushing(false);
				}
				else
					mapEditor->setPushing(false);

				break;
			case Bind::CREATE_WATERBODY:
				if(isPressed) mapEditor->createWaterbody();
				break;
			case Bind::MOVE_TERR_OBJ:
				if(isPressed) mapEditor->setMovingTerrainObject(true);
				break;
			case Bind::STOP_TERR_OBJ:
				if(isPressed) mapEditor->setMovingTerrainObject(false);
				break;
			case Bind::MOVE_X_AXIS:
			case Bind::MOVE_Y_AXIS:
			case Bind::MOVE_Z_AXIS:
				if(isPressed && mapEditor->isMovingTerrainObject()){
					MapEditor::MovementAxis axis = MapEditor::MovementAxis((int)bind - (int)Bind::MOVE_X_AXIS);
					mapEditor->setMovementAxis(axis);
				}

				break;
			case Bind::GENERATE_WEIGHTS:
				mapEditor->prepareTerrainObjects();
				break;
			case Bind::TOGGLE_CELL_MARKERS:
				if(isPressed) mapEditor->toggleCellMarkers();
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
					camCtr->orientCamera(Vector3(0, 1, 0).cross(dirProj), strength);
				}

				break;
			case Bind::LOOK_LEFT: 
			case Bind::LOOK_RIGHT: 
				if(camCtr->isLookingAround())
					camCtr->orientCamera(Vector3(0, 1, 0), strength);

				break;
			case Bind::PUSH_VERTS_UP:
			case Bind::PUSH_VERTS_DOWN:
				if(mapEditor->isPushing()) mapEditor->pushLandmassVerts(strength);
				else if(mapEditor->isMovingTerrainObject() && mapEditor->getSelectedTerrainObject() != &Map::getSingleton()->getTerrainObject(0)) 
					mapEditor->moveTerrainObject(strength);

				break;
		}
	}
}
