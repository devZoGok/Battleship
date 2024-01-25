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

    Projectile::Projectile(Unit *un, int id, Vector3 pos, Quaternion rot) : GameObject(GameObject::Type::PROJECTILE, id, un->getPlayer(), pos, rot), unit(un), initPos(pos){
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

		sol::table projTable = generateView()[GameObject::getGameObjTableName()];
        rayLength = projTable[id + 1]["rayLength"];
        directHitDamage = projTable[id + 1]["directHitDamage"];

		string explKey = "explosion";
        explosionDamage = projTable[id + 1][explKey]["damage"];
        explosionRadius = projTable[id + 1][explKey]["radius"];
        speed = projTable[id + 1]["speed"];
		rotAngle = projTable[id + 1]["rotAngle"];
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

        if(!remove){
			checkSurfaceCollision();
			if(remove) return;
			checkUnitCollision();
		}
    }

	void Projectile::checkUnitCollision(){
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
				if(targetNodes[i]->getMesh(0) == results[0].mesh){
					targetUnits[i]->takeDamage(directHitDamage);
					Game::getSingleton()->explode(pos, explosionDamage, explosionRadius, explosionSfx);
					remove = true;
				}
		}
	}

	void Projectile::checkSurfaceCollision(){
		Map *map = Map::getSingleton();
		int cellId = map->getCellId(pos, false);

		if(map->getCells()[cellId].type == Map::Cell::LAND)
			remove = true;
	}

    void Projectile::debug(){
    }
}
