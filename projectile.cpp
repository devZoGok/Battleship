#include <algorithm>
#include <node.h>
#include <model.h>
#include <material.h>
#include <quaternion.h>
#include <ray.h>

#include "projectile.h"
#include "stateManager.h"
#include "projectileData.h"
#include "inGameAppState.h"
#include "unit.h"
#include "util.h"
#include "explosion.h"

using namespace std;
using namespace vb01;

namespace battleship{
		using namespace configData;

    Projectile::Projectile(Unit *unit, Node *node, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) {
        this->unit=unit;
        this->id=id;
        this->weaponTypeId=weaponTypeId;

        dirVec = dir;
        leftVec = left;
        upVec = up;

        this->rayLength=projectileData::length[id][weaponTypeId][weaponId];

				GameManager *gm = GameManager::getSingleton();
        //this->node=node;

        if(!node){
						/*
            mesh = smgr->getMesh(projectileData::meshPath[id][weaponTypeId]);
            this->node = smgr->addAnimatedMeshSceneNode(mesh);
						*/
						this->node = new Model((projectileData::meshPath[id][weaponTypeId]));
						Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");
						mat->addBoolUniform("lightingEnabled", false);
						string f[]{projectileData::diffuseMapTextPath[id][weaponTypeId]};
            Texture *diffuseTexture = new Texture(f, 1, false);
						mat->addTexUniform("textures[0]", diffuseTexture, true);
						this->node->setMaterial(mat);

						/*
            if (!diffuseTexture)
                diffuseTexture = driver->getTexture(DEFAULT_TEXTURE);
								*/
        }

        this->node->setPosition(pos);
        this->pos = pos;
        this->damage=projectileData::damage[id][weaponTypeId][weaponId];

        initPos = pos;
        speed = projectileData::speed[id][weaponTypeId][weaponId];
        float angle = dirVec.getAngleBetween(Vector3(0, 0, -1));
        angle = Quaternion(angle, upVec) * Vector3(0, 0, -1) == dirVec ? angle : -angle;
        //this->node->setRotation(vector3df(0, angle / PI * 180, 0));
        
        string p1 = PATH + "Sounds/" + unitData::name[id] + "s/" + projectileData::name[id][weaponTypeId] + ".ogg";
        string p2 = PATH + "Sounds/Explosions/explosion03" + to_string(rand() % 4) + ".ogg";
        this->shotSfxBuffer = new sf::SoundBuffer();
        this->explosionSfxBuffer=new sf::SoundBuffer();

        if(shotSfxBuffer->loadFromFile(p1.c_str())){
            shotSfx=new sf::Sound(*shotSfxBuffer);
            shotSfx->play();
        }

        if(explosionSfxBuffer->loadFromFile(p2.c_str()))
            explosionSfx=new sf::Sound(*explosionSfxBuffer);
    }

    Projectile::~Projectile(){
				/*
        ISceneManager *smgr = GameManager::getSingleton()->getDevice()->getSceneManager();
        node->getParent()->removeChild(node);
				*/
    }
    
    void Projectile::update() {
        if(unit->isDebuggable())
            debug();

        checkForCollision();
    }

    void Projectile::checkForCollision() {
        //ISceneNode *collNode = castRay(GameManager::getSingleton()->getSceneManager(),pos,pos+dirVec*rayLength);
				/*
				Ray::castRay(pos, pos + dirVec, );

        if (pos.y < -7 || (collNode&&collNode != unit->getNode()))
            explode(collNode);
						*/
    }

    void Projectile::explode(Node *collNode) {
        exploded = true;
				StateManager *stateManager = GameManager::getSingleton()->getStateManager();
        vector<Player*> players = ((InGameAppState*) stateManager->getAppState(AppStateTypes::IN_GAME_STATE))->getPlayers();

        for (Player *p : players) {
            for (Unit *u : p->getUnits())
                if (collNode == u->getNode())
                    u->takeDamage(damage);
        }

        InGameAppState *inGameState=((InGameAppState*)stateManager->getAppState(AppStateTypes::IN_GAME_STATE));

        if(id == 8)
            detonateTorpedo(inGameState,pos);
        else if(weaponTypeId==1&&(id==2||id==3))
            detonateDepthCharge(inGameState,pos);
        else
            detonate(inGameState,pos,-dirVec);
    }
    
    void Projectile::debug(){
				/*
        IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
        driver->draw3DLine(pos,pos+dirVec,SColor(255,0,0,255));
        driver->draw3DLine(pos,pos+leftVec,SColor(255,255,0,0));
        driver->draw3DLine(pos,pos+upVec,SColor(255,0,255,0));
				*/
    }
    
    void Projectile::orientProjectile(Vector3 orientVec){
        Vector3 orientVecProj = Vector3(orientVec.x, 0, orientVec.z).norm();
        float projAngle = orientVec.getAngleBetween(orientVecProj);
        float rotAngle = Vector3(0,0,-1).getAngleBetween(orientVecProj);
        rotAngle *= (orientVec.x<0?1:-1)/PI*180;
        projAngle*=(orientVec.y>0?1:-1)/PI*180;
        dirVec = orientVecProj;
        upVec = Vector3(0,1,0);
        leftVec = Quaternion(-PI/2, upVec) * dirVec;
        Quaternion rotQuat=Quaternion(projAngle/180*PI,leftVec);
        dirVec=rotQuat*dirVec;
        upVec=rotQuat*upVec;
        //node->setRotation(vector3df(projAngle,rotAngle,0));
    }
}
