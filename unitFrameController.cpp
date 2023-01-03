#include "unitFrameController.h"
#include "util.h"
#include "map.h"

#include <material.h>
#include <meshData.h>
#include <mesh.h>
#include <model.h>
#include <root.h>
#include <ray.h>

#include <luaManager.h>

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

	//TODO implement terrain evenness check
	void UnitFrameController::update(){
		Map *map = Map::getSingleton();
		MeshData meshData = map->getTerrainObject(0).node->getChild(0)->getMesh(0)->getMeshBase();
		MeshData::Vertex *verts = meshData.vertices;
		int numVerts = 3 * meshData.numTris;

		LuaManager *lm = LuaManager::getSingleton();
		float maxUnevenness = lm->getFloat("maxUnevenness");
		string table = "unitCornerPoints";

		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 startPos = cam->getPosition();
		Vector3 endPos = screenToSpace(getCursorPos());

		for(UnitFrame &s : unitFrames){
			vector<Ray::CollisionResult> results;
			Ray::retrieveCollisions(startPos, (endPos - startPos).norm(), map->getTerrainObject(0).node, results);
			Ray::sortResults(results);

			if(!results.empty()){
				if(!rotatingStructure)
					s.model->setPosition(results[0].pos);

    			float width = 
					lm->getFloatFromTable(table, vector<Index>{Index(s.id + 1), Index(1), Index("x")}) -
				   	lm->getFloatFromTable(table, vector<Index>{Index(s.id + 1), Index(2), Index("x")});
    			float length = 
					lm->getFloatFromTable(table, vector<Index>{Index(s.id + 1), Index(4), Index("z")}) -
				   	lm->getFloatFromTable(table, vector<Index>{Index(s.id + 1), Index(1), Index("z")});

				float unevenness = 0;

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
		}
	}

	void UnitFrameController::removeUnitFrames(){
		for(UnitFrame &u : unitFrames){
			u.model->getParent()->dettachChild(u.model);
			delete u.model;
		}

		unitFrames.clear();
	}

	void UnitFrameController::rotateUnitFrames(float angle){
		for(UnitFrame &s : unitFrames)
			if(s.status != UnitFrame::PLACED){
				Quaternion rot = s.model->getOrientation();
			   	s.model->setOrientation(Quaternion(angle, Vector3::VEC_J) * rot);
			}
	}
}
