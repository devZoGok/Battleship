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

		if(hitbox){
			Box *hbMesh = (Box*)hitbox->getMesh(0);
			hbMesh->setSize(Vector3(width, height, length));
			hbMesh->updateVerts(hbMesh->getMeshBase());
		}
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
		box->setWireframe(true);

		Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));
		box->setMaterial(mat);

		sol::table gameObjTable = generateView()[GameObject::getGameObjTableName()][id + 1];
		sol::table offsetPosTable = gameObjTable["hitboxOffset"];
		hitbox = new Node(Vector3(offsetPosTable["x"], offsetPosTable["y"], offsetPosTable["z"]));
		hitbox->attachMesh(box);
		hitbox->setVisible(true);
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

	//TODO improve DEFAULT_TEXTURE handling
	void GameObject::initModel(bool textured){
		sol::table gameObjTable = generateView()[GameObject::getGameObjTableName()][id + 1];
		string basePath = gameObjTable["basePath"];
		string meshPath = gameObjTable["meshPath"];

		model = new Model(basePath + meshPath);
		Root *root = Root::getSingleton();
		string libPath = root->getLibPath();

		Material *mat = new Material(libPath + "texture");

		if(textured){
			string albedoPath = gameObjTable["albedoPath"].get_or(configData::DEFAULT_TEXTURE);
			string f[]{albedoPath == configData::DEFAULT_TEXTURE ? GameManager::getSingleton()->getPath() + albedoPath : basePath + albedoPath};
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
		sol::optional<sol::table> colNodeOpt = gameObjTable["colorNodes"];

		if(player && colNodeOpt != sol::nullopt){
			sol::table colNodeTbl = gameObjTable["colorNodes"];
			int numColorNodes = colNodeTbl.size();

			for(int i = 0; i < numColorNodes; i++){
				string name = colNodeTbl[i + 1];
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
