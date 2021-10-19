#include"MissileJet.h"
#include"ProjectileData.h"
#include"InGameAppState.h"

using namespace game::core;
using namespace game::util;
using namespace irr::scene;

namespace game{
    namespace content{
        MissileJet::MissileJet(GameManager *gM, Player *player, vector3df pos, int id, bool onBoard) : Jet(gM, player, pos, id, onBoard) {}

        void MissileJet::attack(Order order) {
            if(!onBoard&&canFire()&&missilesInstalled){
                vector3df target = *order.targetPos[0];
                float distance=pos.getDistanceFrom(target);
                if(distance <= range){
                    vector3df *targetPtr=nullptr;
                    InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE));
                    for(Player *p : inGameState->getPlayers())
                        for(Unit *u : p->getUnits()){
                            bool jet=u->getType()==UNIT_TYPE::MISSILE_JET||u->getType()==UNIT_TYPE::DEMO_JET;
                            if((jet&&type==AAM)&&(!jet&&type==AWM)&&u->getPosPtr()==order.targetPos[0])
                                targetPtr=u->getPosPtr();
                        }
                    if(!targetPtr){
                        targetPtr=new vector3df();
                        *targetPtr=target;
                    }
                    fireMissile(targetPtr);
                }
                else
                    Unit::attack(order);
            }
            else if(onBoard)
                takeOff();
            else
                removeOrder(0);
        }

        void MissileJet::fireMissile(vector3df *t) {
            ISceneManager *smgr=gameManager->getDevice()->getSceneManager();
            vector3df p = pos + projectileData::pos[id][0][missiles - 1].X * leftVec + projectileData::pos[id][0][missiles - 1].Y * upVec - projectileData::pos[id][0][missiles - 1].Z*dirVec;
            addProjectile(new Missile(gameManager, this, missileNodes[missiles-1], t, p, dirVec, leftVec, upVec, id, 0, 0));
            missileNodes[missiles-1]->setParent(smgr->getRootSceneNode());
            missileNodes[missiles-1]=nullptr;
            missiles--;
            lastFireTime=getTime();
        }
        
        void MissileJet::installMissiles(bool aam){
            if(!missilesInstalled){
                missiles=2;
                ISceneManager *smgr=gameManager->getDevice()->getSceneManager();
                IVideoDriver *driver=gameManager->getDevice()->getVideoDriver();
                for(int i=0;i<missiles;i++){
                    IAnimatedMesh *missileMesh=smgr->getMesh(projectileData::meshPath[id][i]);
                    missileNodes[i]=smgr->addAnimatedMeshSceneNode(missileMesh);
                    ITexture *diffuseTexture = driver->getTexture(projectileData::diffuseMapTextPath[id][i]);
                    if (!diffuseTexture)
                        diffuseTexture = driver->getTexture(DEFAULT_TEXTURE);
                    missileNodes[i]->setMaterialTexture(0, diffuseTexture);
                    missileNodes[i]->setMaterialFlag(EMF_LIGHTING, false);
                    missileNodes[i]->setPosition(projectileData::pos[id][aam][i]);
                    missileNodes[i]->setParent(node);
                }
                
                type=(MissileType)aam;
                missilesInstalled=true;
            }
        }
    }
}
