#include"Projectile.h"
#include"ProjectileData.h"
#include"InGameAppState.h"
#include"Unit.h"
#include"util.h"
#include"Explosion.h"
#include<algorithm>

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::io;
using namespace irr::video;
using namespace std;
using namespace sf;

namespace game{
    namespace content{
        Projectile::Projectile(GameManager *gM, Unit *unit, ISceneNode *node, vector3df pos, vector3df dir, vector3df left, vector3df up, int id, int weaponTypeId, int weaponId) {
            gameManager = gM;
            this->unit=unit;
            this->id=id;
            this->weaponTypeId=weaponTypeId;
            dirVec = dir;
            leftVec = left;
            upVec = up;
            this->rayLength=projectileData::length[id][weaponTypeId][weaponId];
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            this->node=node;
            if(!node){
                mesh = smgr->getMesh(projectileData::meshPath[id][weaponTypeId]);
                this->node = smgr->addAnimatedMeshSceneNode(mesh);
                ITexture *diffuseTexture = driver->getTexture(projectileData::diffuseMapTextPath[id][weaponTypeId]);
                if (!diffuseTexture)
                    diffuseTexture = driver->getTexture(DEFAULT_TEXTURE);
                this->node->setMaterialTexture(0, diffuseTexture);
                this->node->setMaterialFlag(EMF_LIGHTING, false);
            }
            this->node->setPosition(pos);
            this->pos = pos;
            this->damage=projectileData::damage[id][weaponTypeId][weaponId];
            initPos = pos;
            speed = projectileData::speed[id][weaponTypeId][weaponId];
            float angle = getAngleBetween(dirVec, vector3df(0, 0, -1));
            angle = quaternion(0, 0, 0, 1).fromAngleAxis(angle, upVec) * vector3df(0, 0, -1) == dirVec ? angle : -angle;
            this->node->setRotation(vector3df(0, angle / PI * 180, 0));
            
            path p1=PATH+"Sounds/"+unitData::name[id]+"s/"+projectileData::name[id][weaponTypeId]+".ogg";
            path p2=PATH+"Sounds/Explosions/explosion03"+stringw(rand()%4)+".ogg";
            this->shotSfxBuffer=new SoundBuffer();
            this->explosionSfxBuffer=new SoundBuffer();
            if(shotSfxBuffer->loadFromFile(p1.c_str())){
                shotSfx=new Sound(*shotSfxBuffer);
                shotSfx->play();
            }
            if(explosionSfxBuffer->loadFromFile(p2.c_str()))
                explosionSfx=new Sound(*explosionSfxBuffer);
        }

        Projectile::~Projectile(){
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            node->getParent()->removeChild(node);
        }
        
        void Projectile::update() {
            if(unit->isDebuggable())
                debug();
            checkForCollision();
        }

        void Projectile::checkForCollision() {
            ISceneNode *collNode = castRay(gameManager->getSceneManager(),pos,pos+dirVec*rayLength);
            if (pos.Y<-7 || (collNode&&collNode!=unit->getNode()))
                explode(collNode);
        }

        void Projectile::explode(ISceneNode *collNode) {
            exploded = true;
            vector<Player*> players = ((InGameAppState*) gameManager->getAppState(AppStateTypes::IN_GAME_STATE))->getPlayers();
            for (Player *p : players) {
                for (Unit *u : p->getUnits())
                    if (collNode == u->getNode())
                        u->takeDamage(damage);
            }
            InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE));
            if(id==8)
                detonateTorpedo(inGameState,pos);
            else if(weaponTypeId==1&&(id==2||id==3))
                detonateDepthCharge(inGameState,pos);
            else
                detonate(inGameState,pos,-dirVec);
        }
        
        void Projectile::debug(){
            IVideoDriver *driver=gameManager->getDevice()->getVideoDriver();
            driver->draw3DLine(pos,pos+dirVec,SColor(255,0,0,255));
            driver->draw3DLine(pos,pos+leftVec,SColor(255,255,0,0));
            driver->draw3DLine(pos,pos+upVec,SColor(255,0,255,0));
        }
        
        void Projectile::orientProjectile(vector3df orientVec){
            vector3df orientVecProj=vector3df(orientVec.X,0,orientVec.Z).normalize();
            float projAngle=getAngleBetween(orientVec,orientVecProj);
            float rotAngle=getAngleBetween(vector3df(0,0,-1),orientVecProj);
            rotAngle*=(orientVec.X<0?1:-1)/PI*180;
            projAngle*=(orientVec.Y>0?1:-1)/PI*180;
            dirVec=orientVecProj;
            upVec=vector3df(0,1,0);
            leftVec=quaternion(0,0,0,1).fromAngleAxis(-PI/2,upVec)*dirVec;
            quaternion rotQuat=quaternion(0,0,0,1).fromAngleAxis(projAngle/180*PI,leftVec);
            dirVec=rotQuat*dirVec;
            upVec=rotQuat*upVec;
            node->setRotation(vector3df(projAngle,rotAngle,0));
        }
    }
}
