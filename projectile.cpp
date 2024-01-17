#include <node.h>
#include <model.h>
#include <material.h>
#include <particleEmitter.h>
#include <quaternion.h>
#include <rayCaster.h>

#include <stateManager.h>

#include "game.h"
#include "unit.h"
#include "util.h"
#include "projectile.h"
#include "resourceDeposit.h"
#include "defConfigs.h"
#include "inGameAppState.h"
#include "fx.h"

using namespace std;
using namespace vb01;

namespace battleship{
	using namespace vb01;
	using namespace configData;
	using namespace gameBase;

    Projectile::Projectile(Unit *un, int id, Vector3 pos, Quaternion rot) : GameObject(GameObject::Type::PROJECTILE, id, un->getPlayer(), pos, rot), unit(un){
		initProperties();
		initModel();
		initSound();
		placeAt(pos);
		orientAt(rot);
    }

    Projectile::~Projectile(){
		destroyModel();
	}

	void Projectile::initProperties(){
		GameObject::initProperties();

		sol::table SOL_LUA_STATE = generateView()[GameObject::getGameObjTableName()];
        rayLength = SOL_LUA_STATE["rayLength"][id + 1];
        damage = SOL_LUA_STATE["damage"][id + 1];
        speed = SOL_LUA_STATE["speed"][id + 1];
	}

	void Projectile::initSound(){
		GameManager *gm = GameManager::getSingleton();
        explosionSfxBuffer = new sf::SoundBuffer();
        string p2 = gm->getPath() + "Sounds/SFX/Explosions/explosion0" + to_string(rand() % 4) + ".ogg";

        if(explosionSfxBuffer->loadFromFile(p2.c_str()))
            explosionSfx = new sf::Sound(*explosionSfxBuffer);
	}
    
    void Projectile::update() {
		GameObject::update();
		placeAt(pos + speed * dirVec);

        if(!exploded) checkForCollision();
    }

    void Projectile::checkForCollision() {
		Map *map = Map::getSingleton();
		int cellId = map->getCellId(pos);

		if(map->getCells()[cellId].type == Map::Cell::LAND)
			player->removeProjectile(this);

		vector<Player*> players = Game::getSingleton()->getPlayers();
		vector<Unit*> targetUnits;
		vector<Node*> targetNodes;

		for(Player *pl : players){
			vector<Unit*> units = pl->getUnits();

			for(Unit *u : units)
				if(unit && unit != u){
					targetUnits.push_back(u);
					targetNodes.push_back(u->getHitbox());
				}
		}

		vector<RayCaster::CollisionResult> results = RayCaster::cast(pos, dirVec, targetNodes, rayLength);

		if(!results.empty()){
			for(int i = 0; i < targetNodes.size(); i++)
				if(targetNodes[i]->getMesh(0) == results[0].mesh)
					explode(targetUnits[i]);
		}
    }

    void Projectile::explode(Unit *target) {
        exploded = true;
		target->takeDamage(damage);
		Root *root = Root::getSingleton();

		const int numFrames = 1;
		string p[numFrames];

		for(int i = 0; i < numFrames; i++)
			p[i] = GameManager::getSingleton()->getPath() + "Textures/Explosion/explosion07.png";

		Texture *tex = new Texture(p, numFrames, false);

		Material *mat = new Material(root->getLibPath() + "particle");
		mat->addTexUniform("tex", tex, true);

		ParticleEmitter *pe = new ParticleEmitter(1);
		pe->setMaterial(mat);
		pe->setLowLife(3);
		pe->setHighLife(3);
		pe->setSize(10 * Vector2::VEC_IJ);
		pe->setSpeed(0);

		Node *node = new Node(pos + Vector3(0, 2, 0));
		node->attachParticleEmitter(pe);
		node->lookAt(Vector3::VEC_J, Vector3::VEC_K);
		root->getRootNode()->attachChild(node);

		Fx fx(50, 2500, explosionSfx, node);
		fx.activate();
		Game::getSingleton()->addFx(fx);

		player->removeProjectile(this);
    }
    
    void Projectile::debug(){
    }
}
