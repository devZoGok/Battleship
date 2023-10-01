#include <algorithm>

#include <node.h>
#include <model.h>
#include <material.h>
#include <quaternion.h>
#include <ray.h>

#include <stateManager.h>

#include "map.h"
#include "unit.h"
#include "util.h"
#include "projectile.h"
#include "defConfigs.h"
#include "inGameAppState.h"
#include "explosion.h"

using namespace std;
using namespace vb01;

namespace battleship{
	using namespace configData;
	using namespace gameBase;

    Projectile::Projectile(Unit *unit, Node *node, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) : GameObject(id, unit->getPlayer(), pos, Quaternion::QUAT_W){
        this->unit = unit;
        this->id = id;
        this->weaponTypeId = weaponTypeId;
        this->pos = pos;
        initPos = pos;

        dirVec = dir;
        leftVec = left;
        upVec = up;

		initProperties(weaponId);
		initModel(node);
		initSound();
    }

    Projectile::~Projectile(){
    }

	void Projectile::initProperties(int weaponId){
		sol::state_view SOL_LUA_STATE = generateView();
        rayLength = SOL_LUA_STATE["rayLength"][id + 1][weaponTypeId + 1][weaponId + 1];
        damage = SOL_LUA_STATE["damage"][id + 1][weaponTypeId + 1][weaponId + 1];
        speed = SOL_LUA_STATE["speed"][id + 1][weaponTypeId + 1][weaponId + 1];
	}

	void Projectile::initModel(Node *node){
        if(!node){
			sol::state_view SOL_LUA_STATE = generateView();
			string meshPath = SOL_LUA_STATE["meshPath"][id + 1][weaponTypeId + 1];
			this->node = new Model(meshPath);

			Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
			mat->addBoolUniform("lightingEnabled", false);

			string diffTexPath = SOL_LUA_STATE["diffuseMapTextPath"][id + 1][weaponTypeId + 1];
			string f[]{diffTexPath};
            Texture *diffuseTexture = new Texture(f, 1, false);
			mat->addTexUniform("textures[0]", diffuseTexture, true);

			this->node->setMaterial(mat);
        }

        this->node->setPosition(pos);
	}

	void Projectile::initSound(){
		GameManager *gm = GameManager::getSingleton();

		sol::state_view SOL_LUA_STATE = generateView();
		string unitName = SOL_LUA_STATE["name"][id + 1];
		string projectileName = SOL_LUA_STATE["projectileName"][id + 1][weaponTypeId + 1];
        string p1 = gm->getPath() + "Sounds/" + unitName + "s/" + projectileName + ".ogg";
        string p2 = gm->getPath() + "Sounds/Explosions/explosion03" + to_string(rand() % 4) + ".ogg";

        shotSfxBuffer = new sf::SoundBuffer();
        explosionSfxBuffer = new sf::SoundBuffer();

        if(shotSfxBuffer->loadFromFile(p1.c_str())){
            shotSfx = new sf::Sound(*shotSfxBuffer);
            shotSfx->play();
        }

        if(explosionSfxBuffer->loadFromFile(p2.c_str()))
            explosionSfx = new sf::Sound(*explosionSfxBuffer);
	}
    
    void Projectile::update() {
        if(unit->isDebuggable())
            debug();

        checkForCollision();
    }

    void Projectile::checkForCollision() {
    }

    void Projectile::explode(Node *collNode) {
        exploded = true;

        for (Player *p : Map::getSingleton()->getPlayers()) {
            for (Unit *u : p->getUnits())
                if (collNode == u->getNode())
                    u->takeDamage(damage);
        }

		StateManager *sm = GameManager::getSingleton()->getStateManager();
        InGameAppState *inGameState = ((InGameAppState*)sm->getAppStateByType((int)AppStateType::IN_GAME_STATE));

        if(id == 8)
            detonateTorpedo(inGameState, pos);
        else if(weaponTypeId == 1 && (id == 2 || id == 3))
            detonateDepthCharge(inGameState, pos);
        else
            detonate(inGameState, pos, -dirVec);
    }
    
    void Projectile::debug(){
    }
    
    void Projectile::orientProjectile(Quaternion rot){
		node->setOrientation(rot);
		this->rot = rot;
    }
}
