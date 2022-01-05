#include <model.h>

#include <stateManager.h>

#include "missileJet.h"
#include "projectileData.h"
#include "inGameAppState.h"

using namespace vb01;

namespace battleship{
		using namespace unitData;

    MissileJet::MissileJet(Player *player, Vector3 pos, int id, bool onBoard) : Jet(player, pos, id, onBoard) {}

    void MissileJet::attack(Order order) {
        if(!onBoard && canFire() && missilesInstalled){
            Vector3 target = *order.targets[0].pos;
            float distance=pos.getDistanceFrom(target);

            if(distance <= range){
                Vector3 *targetPtr = nullptr;
                InGameAppState *inGameState = ((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

                for(Player *p : inGameState->getPlayers())
                    for(Unit *u : p->getUnits()){
                        bool jet = (u->getType() == UNIT_TYPE::MISSILE_JET || u->getType() == UNIT_TYPE::DEMO_JET);
                        if((jet && type == AAM) && (!jet && type == AWM) && u->getPosPtr() == order.targets[0].pos)
                            targetPtr = u->getPosPtr();
                    }
								
                if(!targetPtr){
                    targetPtr = new Vector3();
                    *targetPtr = target;
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

    void MissileJet::fireMissile(Vector3 *t) {
        Vector3 p = pos + leftVec * projectileData::pos[id][0][missiles - 1].x + upVec * projectileData::pos[id][0][missiles - 1].y - dirVec * projectileData::pos[id][0][missiles - 1].z;
        addProjectile(new Missile(this, missileNodes[missiles-1], t, p, dirVec, leftVec, upVec, id, 0, 0));
        missileNodes[missiles-1]->setParent(Root::getSingleton()->getRootNode());
        missileNodes[missiles-1] = nullptr;
        missiles--;
        lastFireTime=getTime();
    }
    
    void MissileJet::installMissiles(bool aam){
				/*
        if(!missilesInstalled){
            missiles=2;
						GameManager *gm = GameManager::getSingleton();
            ISceneManager *smgr = gm->getDevice()->getSceneManager();
            IVideoDriver *driver = gm->getDevice()->getVideoDriver();

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
				*/
    }
}
