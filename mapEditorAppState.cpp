#include "mapEditorAppState.h"
#include "gameManager.h"
#include "defConfigs.h"
#include "cameraController.h"
#include "unitFrameController.h"
#include "player.h"
#include "map.h"
#include "mesh.h"
#include "util.h"

#include <ray.h>
#include <box.h>
#include <text.h>
#include <node.h>
#include <quad.h>
#include <model.h>
#include <texture.h>
#include <assetManager.h>

#include <util.h>

#include <listbox.h>

#include <map>
#include <filesystem>
#include <filesystem>

#include <tinyxml2.h>

namespace battleship{
	using namespace tinyxml2;
	using namespace configData;
	using namespace gameBase;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace std;
	using namespace std::filesystem;

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

		string basePath = GameManager::getSingleton()->getPath();
		prepareTextures(basePath + "Textures/Skyboxes/", true, skyTextures);
		prepareTextures(basePath + "Textures/Landmass/", false, landmassTextures);
		prepareTextures(basePath + "Textures/Water/", false, waterTextures);

		AssetManager *assetManager = AssetManager::getSingleton();
		assetManager->load(basePath + "Models/Units/", true);
		assetManager->load(basePath + DEFAULT_TEXTURE);
	}

	void MapEditorAppState::MapEditor::updateCircleRadius(bool increase){
		circleRadius += (increase ? INCREASE_RATE : -INCREASE_RATE);

		if(circleRadius > MAX_RADIUS)
			circleRadius = MAX_RADIUS;
		else if(circleRadius < MIN_RADIUS)
			circleRadius = MIN_RADIUS;
	}

	void MapEditorAppState::MapEditor::toggleSelection(Node *terrNode, bool select){
		terrNode->getMesh(0)->setWireframe(select);
		selectedTerrainNode = (select ? terrNode : nullptr);
	}

	void MapEditorAppState::MapEditor::castSelectionRay(){
	/*
		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 startPos = cam->getPosition();
		Vector3 endPos = screenToSpace(getCursorPos());

		vector<Ray::CollisionResult> results;
		Map *map = Map::getSingleton();
		Ray::retrieveCollisions(startPos, (endPos - startPos).norm(), map->getNodeParent(), results);
		Ray::sortResults(results);

		if(selectedTerrainNode)
			toggleSelection(selectedTerrainNode, false);

		if(!results.empty()){
			for(int i = 0; i < map->getNodeParent()->getNumChildren(); i++)
				if(map->getNodeParent()->getChild(i) == results[i].mesh->getNode()){
					toggleSelection(map->getNodeParent()->getChild(i), true);
					break;
				}
		}
	*/
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
		Node *node = new Node(pos);
		node->attachMesh(quad);

		Map *map = Map::getSingleton();
		Node *terrNodeParent = map->getNodeParent();
		terrNodeParent->attachChild(node);
		toggleSelection(node, true);
	}

	void MapEditorAppState::MapEditor::moveTerrainObject(float strength){
		Vector3 pos = selectedTerrainNode->getPosition();

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

		selectedTerrainNode->setPosition(pos);
	}

	void MapEditorAppState::MapEditor::pushLandmassVerts(float strength){
		Mesh *mesh = map->getNodeParent()->getChild(0)->getMesh(0);
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
		map->getNodeParent()->attachChild(node);
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

	void MapEditorAppState::MapEditor::generateLandmassXml(){
		XMLDocument *doc = new XMLDocument();

		char *nodeTagName = "node";
		XMLElement *rootEl = doc->NewElement(nodeTagName);
		rootEl->SetAttribute("name", "terrain");
		XMLNode *rootTag = doc->InsertEndChild(rootEl);

		XMLElement *nodeEl = doc->NewElement(nodeTagName);
		nodeEl->SetAttribute("name", "plane");
		XMLNode *nodeTag = rootTag->InsertEndChild(nodeEl);

		XMLElement *meshEl = doc->NewElement("mesh");
		MeshData meshData = map->getNodeParent()->getChild(0)->getMesh(0)->getMeshBase();
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

	vector<Map::Cell> MapEditorAppState::MapEditor::generateMapCells(){
		Vector3 startPos = -.49 * Vector3(mapSize.x, 0, mapSize.y), cellSize = map->getCellSize();
		int numHorCells = int(mapSize.x / cellSize.x);
		int numVertCells = int(mapSize.y / cellSize.z);
		vector<Map::Cell> cells;
		vector<pair<int, float>> waterBodyBedPoints;
		Node *terrainNode = map->getNodeParent();

		for(int i = 0; i < numVertCells; i++)
			for(int j = 0; j < numHorCells; j++){
				vector<Ray::CollisionResult> res;
				Vector3 rayPos = startPos + Vector3(cellSize.x * j, 100, cellSize.z * i);
				Ray::retrieveCollisions(rayPos, -Vector3::VEC_J, terrainNode->getChild(0), res);
				Ray::sortResults(res);

				if(res.empty()){
					Ray::CollisionResult r;
					r.pos = Vector3(rayPos.x, 0, rayPos.z);
					res.push_back(r);
				}
				
				Map::Cell::Type type = Map::Cell::Type::LAND;
				Vector3 pos = res[0].pos;

				for(int k = 1; k < terrainNode->getNumChildren(); k++){
					Vector3 waterPos = terrainNode->getChild(k)->getPosition();
					Vector3 waterSize = ((Quad*)terrainNode->getChild(k)->getMesh(0))->getSize();

					if(fabs(res[0].pos.x - waterPos.x) < .5 * waterSize.x && fabs(res[0].pos.z - waterPos.z) < .5 * waterSize.y){
						type = Map::Cell::Type::WATER;
						pos.y = waterPos.y;
						waterBodyBedPoints.push_back(pair(numVertCells * i + j, res[0].pos.y));
						break;
					}
				}

				vector<Map::Edge> edges;
				int weight = 1;
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
					edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * (i - 1) + j - 1));

				if(up && right)
					edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * (i - 1) + j + 1));

				if(down && left)
					edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * (i + 1) + j - 1));

				if(down && right)
					edges.push_back(Map::Edge(weight, numVertCells * i + j, numVertCells * (i + 1) + j + 1));

				cells.push_back(Map::Cell(pos, type, edges));
			}

		vector<Map::Cell> waterCells;
		int currUnderWaterCellId = cells.size();
		int weight = 2;

		for(pair<int, float> p : waterBodyBedPoints){
			int numUnderWaterCells = (int)((cells[p.first].pos.y - p.second) / cellSize.y);

			if(numUnderWaterCells > 0){
				cells[p.first].edges.push_back(Map::Edge(weight, p.first, currUnderWaterCellId));

				for(int i = 0; i < numUnderWaterCells; i++, currUnderWaterCellId++)
					cells[p.first].underWaterCellIds.push_back(currUnderWaterCellId);

				waterCells.push_back(cells[p.first]);
			}
		}

		for(int i = 0; i < waterCells.size(); i++){
			for(int j = 0; j < waterCells[i].underWaterCellIds.size(); j++){
				int aboveCellId = (j == 0 ? waterCells[i].edges[0].srcCellId : j - 1);
				vector<Map::Edge> edges = vector<Map::Edge>{Map::Edge(weight, waterCells[i].underWaterCellIds[j], aboveCellId)};

				for(int k = 0; k < waterCells[i].edges.size(); k++){
					Map::Cell adjacentUnderwaterCell = cells[waterCells[i].edges[k].destCellId]; 

					if(adjacentUnderwaterCell.underWaterCellIds.size() >= j - 1){
						edges.push_back(Map::Edge(weight, waterCells[i].underWaterCellIds[j], adjacentUnderwaterCell.underWaterCellIds[j]));
					}
				}

				Vector3 cellPos = waterCells[i].pos - Vector3::VEC_J * cellSize.y * (j + 1);
				cells.push_back(Map::Cell(cellPos, Map::Cell::Type::WATER, edges));
			}
		}

		return cells;
	}

	void MapEditorAppState::MapEditor::generateMapScript(){
		int numWaterBodies = map->getNodeParent()->getNumChildren() - 1;
		string mapScript = "map = {\nnumWaterBodies = " + to_string(numWaterBodies) + ",\n";
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
			mapScript += "{\nspawnPoint = 0,\n";

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
			}

			mapScript += "}\n";
		}

		vector<Map::Cell> cells = generateMapCells();
		mapScript += "},\nnumCells = " + to_string(cells.size()) + ",\n";
		mapScript += "cells = {\n";

		for(Map::Cell cell : cells){
			Vector3 p = cell.pos;
			mapScript += "{type = " + to_string((int)cell.type) + ", pos = {x = " + to_string(p.x) + ", y = " + to_string(p.y) + ", z = " + to_string(p.z) + "}, numEdges = " + to_string(cell.edges.size()) + ", edges = {";

			for(Map::Edge edge : cell.edges)
				mapScript += "{srcCellId = " + to_string(edge.srcCellId) + ", destCellId = "  + to_string(edge.destCellId) + ", weight = "  + to_string(edge.weight) + "}, ";

			mapScript += "}\n},\n";
		}

		mapScript += "},\n";

		Root *root = Root::getSingleton();
		string skyboxPath = "";

		if(root->getSkybox()){
			Texture *tex = ((Material::TextureUniform*)root->getSkybox()->getMaterial()->getUniform("tex"))->value;
			string path = tex->getPath()[0];
			int slashId = path.find_last_of('/');
			skyboxPath = path.substr(0, slashId);

			slashId = skyboxPath.find_last_of('/');
			skyboxPath = skyboxPath.substr(slashId + 1);
		}

		mapScript += "skybox = \"" + skyboxPath + "\",\n";
		mapScript += "terrain = {model = \"" + map->getMapName()  + ".xml\", albedo = \"" + map->getMapName() + ".jpg\"},\n";
		mapScript += "waterbodies = {\n";

		for(int i = 0; i < numWaterBodies; i++){
			Node *waterNode = map->getNodeParent()->getChild(i + 1);
			Vector3 pos = waterNode->getPosition();
			Vector3 size = ((Quad*)waterNode->getMesh(0))->getSize();
			mapScript += 
				"{pos = {x = " + to_string(pos.x) + ", y = " + to_string(pos.y) + ", z = " + to_string(pos.z) + "},\
				size = {x = " + to_string(size.x) + ", y = " + to_string(size.y) + "}, albedo = \"water.jpg\"},";
		}

		mapScript += "}\n}";

		string file = GameManager::getSingleton()->getPath() + "Models/Maps/" + map->getMapName() + "/" + map->getMapName() + ".lua";

		std::ofstream outFile(file);
		outFile << mapScript;
		outFile.close();
	}

	void MapEditorAppState::MapEditor::exportMap(){
		string assetsPath = GameManager::getSingleton()->getPath();
		string mapFolder = assetsPath + "Models/Maps/" + map->getMapName() + "/";
		create_directory(mapFolder);
		copy_file(assetsPath + DEFAULT_TEXTURE, mapFolder + map->getMapName() + ".jpg");

		generateLandmassXml();
		generateMapScript();
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
					Ray::retrieveCollisions(startPos, (endPos - startPos).norm(), Root::getSingleton()->getRootNode(), results);
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
				else if(mapEditor->isMovingTerrainObject() && mapEditor->getSelectedNode() != Map::getSingleton()->getNodeParent()->getChild(0)) 
					mapEditor->moveTerrainObject(strength);

				break;
		}
	}
}
