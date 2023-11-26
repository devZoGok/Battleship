#include <solUtil.h>

#include "gameObjectFrameController.h"
#include "unit.h"
#include "util.h"
#include "map.h"

#include <material.h>
#include <meshData.h>
#include <mesh.h>
#include <model.h>
#include <root.h>
#include <rayCaster.h>

#include <string>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	static GameObjectFrameController *gameObjectFrameController = nullptr;

	GameObjectFrameController* GameObjectFrameController::getSingleton(){
		if(!gameObjectFrameController)
			gameObjectFrameController = new GameObjectFrameController();

		return gameObjectFrameController;
	}

	void GameObjectFrameController::paintSelect(Vector3 rowEnd, float width, float length){
		Vector3 rowDir = rowEnd - paintSelectRowStart;
		float lenRow = rowDir.getLength();
		float hypothenuse = sqrt(width * width + length * length);
		int numStructsInRow = int(lenRow / hypothenuse);
		
		while(gameObjectFrames.size() > numStructsInRow)
			removeGameObjectFrame(gameObjectFrames.size() - 1);

		if(gameObjectFrames.size() < numStructsInRow){
			int structureId = gameObjectFrames[0].getId();
			Vector3 buildDir = rowDir; 

			if(gameObjectFrames.size() > 1)
				buildDir = gameObjectFrames[1].getModel()->getPosition() - gameObjectFrames[0].getModel()->getPosition();

			Vector3 pos = paintSelectRowStart + buildDir.norm() * hypothenuse * gameObjectFrames.size();
			addGameObjectFrame(GameObjectFrame(structureId, GameObject::Type::UNIT, pos));
		}
	}

	//TODO implement terrain evenness check
	void GameObjectFrameController::placeGameObjectFrame(int id, Vector3 newPos, float width, float length){
		GameObjectFrame &s = gameObjectFrames[id];

		if(!rotatingStructure)
			s.placeAt(newPos);

		Map *map = Map::getSingleton();
		MeshData meshData = map->getNodeParent()->getChild(0)->getMesh(0)->getMeshBase();
		MeshData::Vertex *verts = meshData.vertices;
		int numVerts = 3 * meshData.numTris;

		float maxUnevenness = generateView()["maxUnevenness"], unevenness = 0;

		for(int i = 0; i < numVerts; i++){
			float diffX = fabs(s.getPos().x - verts[i].pos.x);
			float diffY = fabs(s.getPos().y - verts[i].pos.y);
			float diffZ = fabs(s.getPos().z - verts[i].pos.z);

			if(diffX < 0.5 * width && diffZ < 0.5 * length && diffY > unevenness){
				unevenness = diffY;

				if(unevenness > maxUnevenness)
					break;
			}
		}

		Vector4 color;

		if(unevenness > maxUnevenness){
			color = Vector4(1, 0, 0, 1);
			s.status = GameObjectFrame::NOT_PLACEABLE;
		}
		else{
			color = Vector4(0, 1, 0, 1);
			s.status = GameObjectFrame::PLACEABLE;
		}

		Material *mat = s.getModel()->getMaterial();
		mat->setVec4Uniform("diffuseColor", color);
	}

	void GameObjectFrameController::update(){
		for(GameObjectFrame &frame : gameObjectFrames)
			frame.update();

		Vector3 startPos = Root::getSingleton()->getCamera()->getPosition();
		vector<RayCaster::CollisionResult> results = RayCaster::cast(startPos, (screenToSpace(getCursorPos()) - startPos).norm(), Map::getSingleton()->getNodeParent()->getChild(0));

		if(results.empty()) return;

		Vector3 newPos, rowEnd;
		sol::table unitTable = generateView()[gameObjectFrames[0].getGameObjTableName()]["unitCornerPoints"][gameObjectFrames[0].getId() + 1]; 
    	float width = (float)unitTable[1]["x"] - (float)unitTable[2]["x"];
    	float length = (float)unitTable[4]["z"] - (float)unitTable[1]["z"];

		if(paintSelecting)
			paintSelect(results[0].pos, width, length);
		else if(!(paintSelecting || rotatingStructure))
			newPos = results[0].pos;

		for(int i = 0; i < gameObjectFrames.size(); i++){
			if(paintSelecting){
				float hypothenuse = sqrt(width * width + length * length);
				Vector3 dir = (results[0].pos - paintSelectRowStart).norm(); 
				newPos = paintSelectRowStart + dir * hypothenuse * i;
			}

			placeGameObjectFrame(i, newPos, width, length);
		}
	}

	void GameObjectFrameController::removeGameObjectFrame(int i){
		gameObjectFrames[i].destroy();
		gameObjectFrames.erase(gameObjectFrames.begin() + i);
	}

	void GameObjectFrameController::removeGameObjectFrames(){
		while(gameObjectFrames.size() > 0)
			removeGameObjectFrame(0);
	}

	void GameObjectFrameController::rotateGameObjectFrames(float angle){
		for(GameObjectFrame &s : gameObjectFrames)
			if(s.status != GameObjectFrame::PLACED)
			   	s.orientAt(Quaternion(angle, Vector3::VEC_J) * s.getRot());
	}
}
