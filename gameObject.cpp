#include "gameObject.h"
#include "gameManager.h"
#include "defConfigs.h"
#include "unit.h"
#include "player.h"

#include <solUtil.h>

#include <material.h>
#include <box.h>

#include <SFML/Audio.hpp>

#include <string>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;
	using namespace configData;

	GameObject::GameObject(Type t, int i, Player *pl, vb01::Vector3 vec, vb01::Quaternion quat) : type(t), id(i), player(pl), pos(vec), rot(quat){}

	void GameObject::reinit(){
		placeAt(pos);
		orientAt(rot);
	}

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

	//TODO remove neccessity to create a material for an invisible mesh
	void GameObject::initHitbox(){
		Box *box = new Box(Vector3(width, height, length));
		box->setMaterial(new Material(Root::getSingleton()->getLibPath() + "texture"));
		box->setWireframe(false);

		sol::table gameObjTable = generateView()[GameObject::getGameObjTableName()][id + 1];
		sol::table offsetPosTable = gameObjTable["hitboxOffset"];
		hitbox = new Node(Vector3(offsetPosTable["x"], offsetPosTable["y"], offsetPosTable["z"]));
		hitbox->attachMesh(box);
		hitbox->setVisible(false);
		model->attachChild(hitbox);
	}

	void GameObject::destroyHitbox(){
		hitbox->dettachMesh(0);
		model->dettachChild(hitbox);
		delete hitbox;
	}

	void GameObject::initProperties(){
		sol::table sizeTable = generateView()[GameObject::getGameObjTableName()][id + 1]["size"];
        width = sizeTable["x"];
        height = sizeTable["y"];
        length = sizeTable["z"];
	}
	
	void GameObject::destroyModel(){
		Root::getSingleton()->getRootNode()->dettachChild(model);
		delete model;
	}

	void GameObject::initModel(bool textured){
		sol::table gameObjTable = generateView()[GameObject::getGameObjTableName()][id + 1];
		string basePath = gameObjTable["basePath"];
		string meshPath = gameObjTable["meshPath"];

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

		if((sol::optional<sol::table>)gameObjTable["colorNodes"] != sol::nullopt){
			int numColorNodes = ((sol::table)gameObjTable["colorNodes"]).size();

			for(int i = 0; i < numColorNodes; i++){
				string name = gameObjTable["colorNodes"][i + 1];
				Node *node = model->findDescendant(name, true);

				if(!node) continue;

				vector<Mesh*> meshes = node->getMeshes();

				for(Mesh *mesh : meshes)
					mesh->setMaterial(player->getColorMaterial());
			}
		}

		root->getRootNode()->attachChild(model);
	}

	//TODO implement death SFX destruction 
	void GameObject::destroySound(){
	}

	sf::Sound* GameObject::prepareSfx(sf::SoundBuffer *buffer, string sfxPath){
		sf::Sound *sfx = nullptr;

        if(buffer->loadFromFile(sfxPath.c_str())){
            sfx = new sf::Sound(*buffer);
			sfx->setBuffer(*buffer);
		}

		return sfx;
	}

	void GameObject::initSound(){
		deathSfxBuffer = new sf::SoundBuffer();
		string sfxPath = generateView()[getGameObjTableName()][id + 1]["deathSfx"];
		deathSfx = prepareSfx(deathSfxBuffer, sfxPath);
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
		Vector3 corners[NUM_CORNERS]{
			Vector3(-.5 * width, -.5 * height, -.5 * length),
			Vector3(-.5 * width, -.5 * height, .5 * length),
			Vector3(.5 * width, -.5 * height, .5 * length),
			Vector3(.5 * width, -.5 * height, -.5 * length),
			Vector3(-.5 * width, .5 * height, -.5 * length),
			Vector3(-.5 * width, .5 * height, .5 * length),
			Vector3(.5 * width, .5 * height, .5 * length),
			Vector3(.5 * width, .5 * height, -.5 * length)
		};

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

	void GameObject::updateGameStats(Unit *targetUnit){
		if(targetUnit->getHealth() <= targetUnit->getDeathHp()){
			Player *targUnitPlayer = targetUnit->getPlayer();

			if(targetUnit->isVehicle()){
				player->incVehiclesDestroyed();
				targUnitPlayer->incVehiclesLost();
			}
			else{
				player->incStructuresDestroyed();
				targUnitPlayer->incStructuresLost();
			}
		}
	}
}
