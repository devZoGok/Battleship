#include <algorithm>

#include <node.h>
#include <model.h>
#include <material.h>
#include <quaternion.h>
#include <ray.h>

#include <stateManager.h>
#include <luaManager.h>

#include "projectile.h"
#include "defConfigs.h"
#include "inGameAppState.h"
#include "unit.h"
#include "util.h"
#include "explosion.h"

using namespace std;
using namespace vb01;

namespace battleship{
	using namespace configData;
	using namespace gameBase;

    Projectile::Projectile(Unit *unit, Node *node, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) {
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
		LuaManager *lm = LuaManager::getSingleton();
		string pathBase = GameManager::getSingleton()->getPath() + "Scripts/";
		lm->buildScript(vector<string>{pathBase + "defPaths.lua", pathBase + "unitData.lua", pathBase + "projectileData.lua"});

		vector<Index> indices = vector<Index>{Index(id + 1), Index(weaponTypeId + 1), Index(weaponId + 1)};
        rayLength = lm->getFloatFromTable("rayLength", indices);
        damage = lm->getIntFromTable("damage", indices);
        speed = lm->getFloatFromTable("speed", indices);
	}

	void Projectile::initModel(Node *node){
        if(!node){
			LuaManager *lm = LuaManager::getSingleton();
			vector<Index> indices = vector<Index>{Index(id + 1), Index(weaponTypeId + 1)};
			string meshPath = lm->getStringFromTable("meshPath", indices);
			this->node = new Model(meshPath);

			Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
			mat->addBoolUniform("lightingEnabled", false);

			string diffTexPath = lm->getStringFromTable("diffuseMapTextPath", indices);
			string f[]{diffTexPath};
            Texture *diffuseTexture = new Texture(f, 1, false);
			mat->addTexUniform("textures[0]", diffuseTexture, true);

			this->node->setMaterial(mat);
        }

        this->node->setPosition(pos);
	}

	void Projectile::initSound(){
		GameManager *gm = GameManager::getSingleton();
		LuaManager *lm = LuaManager::getSingleton();

		string unitName = lm->getStringFromTable("name", vector<Index>{Index(id + 1)});
		string projectileName = lm->getStringFromTable("projectileName", vector<Index>{Index(id + 1), Index(weaponTypeId + 1)});
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
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
        vector<Player*> players = ((InGameAppState*) stateManager->getAppStateByType((int)AppStateType::IN_GAME_STATE))->getPlayers();

        for (Player *p : players) {
            for (Unit *u : p->getUnits())
                if (collNode == u->getNode())
                    u->takeDamage(damage);
        }

        InGameAppState *inGameState = ((InGameAppState*)stateManager->getAppStateByType((int)AppStateType::IN_GAME_STATE));

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
