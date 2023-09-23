#include <solUtil.h>

#include "unitFrameController.h"
#include "unit.h"
#include "util.h"
#include "map.h"

#include <material.h>
#include <meshData.h>
#include <mesh.h>
#include <model.h>
#include <root.h>
#include <ray.h>

#include <string>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	static UnitFrameController *unitFrameController = nullptr;

	UnitFrameController::UnitFrame::UnitFrame(string modelPath, int i, int t) : id(i), type(t) {
		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addBoolUniform("lightingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4::VEC_ZERO);

		model = new Model(modelPath);
		model->setWireframe(true);
		model->setMaterial(mat);
		root->getRootNode()->attachChild(model);
	}    

	UnitFrameController* UnitFrameController::getSingleton(){
		if(!unitFrameController)
			unitFrameController = new UnitFrameController();

		return unitFrameController;
	}

	void UnitFrameController::paintSelect(Vector3 rowEnd, float width, float length){
		float lenRow = (rowEnd - paintSelectRowStart).getLength();
		float hypothenuse = sqrt(width * width + length * length);
		int numStructsInRow = int(lenRow / hypothenuse);
		
		while(unitFrames.size() > numStructsInRow)
			removeUnitFrame(unitFrames.size() - 1);

		if(unitFrames.size() < numStructsInRow){
			int structureId = unitFrames[0].id;
			sol::state_view SOL_LUA_STATE = generateView();
			string modelPath = (string)SOL_LUA_STATE["basePath"][structureId + 1] + (string)SOL_LUA_STATE["meshPath"][structureId + 1];
			addUnitFrame(UnitFrame(modelPath, structureId, (int)UnitType::LAND));
		}
	}

	//TODO implement terrain evenness check
	void UnitFrameController::placeUnitFrame(int id, Vector3 newPos, float width, float length){
		UnitFrame &s = unitFrames[id];
		s.model->setPosition(newPos);

		Map *map = Map::getSingleton();
		MeshData meshData = map->getNodeParent()->getChild(0)->getMesh(0)->getMeshBase();
		MeshData::Vertex *verts = meshData.vertices;
		int numVerts = 3 * meshData.numTris;

		float maxUnevenness = generateView()["maxUnevenness"], unevenness = 0;

		for(int i = 0; i < numVerts; i++){
			float diffX = fabs(s.model->getPosition().x - verts[i].pos.x);
			float diffY = fabs(s.model->getPosition().y - verts[i].pos.y);
			float diffZ = fabs(s.model->getPosition().z - verts[i].pos.z);

			if(diffX < 0.5 * width && diffZ < 0.5 * length && diffY > unevenness){
				unevenness = diffY;

				if(unevenness > maxUnevenness)
					break;
			}
		}

		Vector4 color;

		if(unevenness > maxUnevenness){
			color = Vector4(1, 0, 0, 1);
			s.status = UnitFrame::NOT_PLACEABLE;
		}
		else{
			color = Vector4(0, 1, 0, 1);
			s.status = UnitFrame::PLACEABLE;
		}

		Material *mat = s.model->getMaterial();
		mat->setVec4Uniform("diffuseColor", color);
	}

	void UnitFrameController::update(){
		Vector3 startPos = Root::getSingleton()->getCamera()->getPosition();
		vector<Ray::CollisionResult> results;
		Ray::retrieveCollisions(startPos, (screenToSpace(getCursorPos()) - startPos).norm(), Map::getSingleton()->getNodeParent()->getChild(0), results);
		Ray::sortResults(results);
		if(results.empty()) return;

		Vector3 newPos, rowEnd;
		sol::state_view SOL_LUA_STATE = generateView();
		sol::table unitTable = SOL_LUA_STATE["unitCornerPoints"][unitFrames[0].id + 1]; 
    	float width = (float)unitTable[1]["x"] - (float)unitTable[2]["x"];
    	float length = (float)unitTable[4]["z"] - (float)unitTable[1]["z"];

		if(paintSelecting)
			paintSelect(results[0].pos, width, length);
		else if(!(paintSelecting || rotatingStructure))
			newPos = results[0].pos;

		for(int i = 0; i < unitFrames.size(); i++){
			if(paintSelecting){
				float hypothenuse = sqrt(width * width + length * length);
				Vector3 dir = (results[0].pos - paintSelectRowStart).norm(); 
				newPos = paintSelectRowStart + dir * hypothenuse * i;
			}

			placeUnitFrame(i, newPos, width, length);
		}
	}

	void UnitFrameController::removeUnitFrame(int i){
		unitFrames[i].model->getParent()->dettachChild(unitFrames[i].model);
		delete unitFrames[i].model;
		unitFrames.erase(unitFrames.begin() + i);
	}

	void UnitFrameController::removeUnitFrames(){
		while(unitFrames.size() > 0)
			removeUnitFrame(0);
	}

	void UnitFrameController::rotateUnitFrames(float angle){
		for(UnitFrame &s : unitFrames)
			if(s.status != UnitFrame::PLACED){
				Quaternion rot = s.model->getOrientation();
			   	s.model->setOrientation(Quaternion(angle, Vector3::VEC_J) * rot);
			}
	}
}
