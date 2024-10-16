#include "gameObject.h"
#include "gameManager.h"
#include "defConfigs.h"

#include <solUtil.h>

#include <material.h>

#include <SFML/Audio.hpp>

#include <string>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;
	using namespace configData;

	void GameObject::update(){
		leftVec = model->getGlobalAxis(0);
		upVec = model->getGlobalAxis(1);
		dirVec = model->getGlobalAxis(2);
		screenPos = spaceToScreen(pos);
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
		sol::table SOL_LUA_STATE = generateView()[GameObject::getGameObjTableName()];

		for(int i = 0; i < 8; i++){
			sol::table cornerTable = SOL_LUA_STATE["unitCornerPoints"][id + 1][i + 1];
			corners[i] = Vector3(cornerTable["x"], cornerTable["y"], cornerTable["z"]);
		}

        width = corners[0].x - corners[1].x;
        height = corners[4].y - corners[0].y;
        length = corners[3].z - corners[0].z;
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

	void GameObject::destroySound(){
	}

	sf::Sound* GameObject::prepareSfx(sf::SoundBuffer *buffer, string key){
		sol::table SOL_LUA_STATE = generateView()[GameObject::getGameObjTableName()];
        string sfxPath = SOL_LUA_STATE[key][id + 1];

		sf::Sound *sfx = nullptr;

        if(buffer->loadFromFile(sfxPath.c_str())){
            sfx = new sf::Sound(*buffer);
			sfx->setBuffer(*buffer);
		}

		return sfx;
	}

	void GameObject::initSound(){
		deathSfxBuffer = new sf::SoundBuffer();
		deathSfx = prepareSfx(deathSfxBuffer, "deathSfx");
	}

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

	int GameObject::sortCorners(vector<Vector2> &cornersOnScreen, bool vertical, bool max){
		const int NUM_CORNERS = cornersOnScreen.size();
		int ans = 0;

		for(int i = 0; i < NUM_CORNERS; i++){
			if(vertical && ((max && cornersOnScreen[ans].y < cornersOnScreen[i].y) || (!max && cornersOnScreen[ans].y > cornersOnScreen[i].y)))
				ans = i;
			else if(!vertical && ((max && cornersOnScreen[ans].x < cornersOnScreen[i].x) || (!max && cornersOnScreen[ans].x > cornersOnScreen[i].x)))
				ans = i;
		}

		return ans;
	}

	Vector2 GameObject::calculateSelectionRect(){
		const int NUM_CORNERS = 8;
		vector<Vector2> cornersOnScreen;

		for(int i = 0; i < NUM_CORNERS; i++){
			Vector3 cornerInWorld = leftVec * corners[i].x + upVec * corners[i].y + dirVec * corners[i].z;
			cornersOnScreen.push_back(spaceToScreen(cornerInWorld));
		}

		int leftMostPointId = sortCorners(cornersOnScreen, false, false);
		int rightMostPointId = sortCorners(cornersOnScreen, false, true);
		int topMostPointId = sortCorners(cornersOnScreen, true, false);
		int bottomMostPointId = sortCorners(cornersOnScreen, true, true);

		float sizeX = cornersOnScreen[rightMostPointId].x - cornersOnScreen[leftMostPointId].x;
		float sizeY = cornersOnScreen[bottomMostPointId].y - cornersOnScreen[topMostPointId].y;
		return Vector2(sizeX, sizeY);
	}
}
