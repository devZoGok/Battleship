#include "gameObject.h"
#include "gameManager.h"
#include "defConfigs.h"

#include <solUtil.h>

#include <material.h>

#include <string>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;
	using namespace configData;

	void GameObject::update(){
	}

    void GameObject::placeAt(Vector3 p) {
        model->setPosition(p);
        pos = p;
    }

    void GameObject::orientAt(Quaternion rotQuat){
		model->setOrientation(rotQuat);
		rot = rotQuat;
    }

	void GameObject::initProperties(){
	}
	
	void GameObject::destroyModel(){
		Root::getSingleton()->getRootNode()->dettachChild(model);
		delete model;
	}

	void GameObject::initModel(bool textured){
		sol::table SOL_LUA_STATE = generateView()[GameObject::getGameObjTableName()];
		string basePath = SOL_LUA_STATE["basePath"][id + 1];
		string meshPath = SOL_LUA_STATE["meshPath"][id + 1];

		model = new Model(basePath + meshPath);
		Root *root = Root::getSingleton();
		string libPath = root->getLibPath();

		Material *mat = new Material(libPath + "texture");

		if(textured){
			string f[]{GameManager::getSingleton()->getPath() + configData::DEFAULT_TEXTURE};
    		Texture *diffuseTexture = new Texture(f, 1, false);
			mat->addBoolUniform("texturingEnabled", true);
			mat->addBoolUniform("lightingEnabled", false);
			mat->addTexUniform("textures[0]", diffuseTexture, true);
		}
		else{
			mat->addBoolUniform("texturingEnabled", false);
			mat->addBoolUniform("lightingEnabled", false);
			mat->addVec4Uniform("diffuseColor", Vector4::VEC_ZERO);
			model->setWireframe(true);
		}

		model->setMaterial(mat);
		root->getRootNode()->attachChild(model);
	}

	void GameObject::destroySound(){}

	void GameObject::initSound(){}

	string GameObject::getGameObjTableName(){
		switch(type){
			case GameObject::Type::UNIT:
				return "units";
			case GameObject::Type::PROJECTILE:
				return "projectiles";
			case GameObject::Type::RESOURCE_DEPOSIT:
				return "resources";
		}
	}
}
