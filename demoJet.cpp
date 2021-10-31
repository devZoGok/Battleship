#include <quaternion.h>

#include "demoJet.h"
#include "inGameAppState.h"
#include "stateManager.h"
#include "util.h"

using namespace game::core;
using namespace game::util;
using namespace vb01;

namespace game{
    namespace content{
        DemoJet::DemoJet(Player *player, Vector3 pos, int id, bool onBoard) : Jet(player, pos, id,onBoard) {}
        
        void DemoJet::attack(Order order){
            if(!onBoard){
                Unit::attack(order);
                Vector3 target = *order.targetPos[0];
                float angle = dirVec.getAngleBetween(target - pos);
                Vector3 axis = dirVec.cross(target - pos);

                if(maxTurnAngle < angle / PI * 180){
                    Quaternion rotQuat = Quaternion(maxTurnAngle / 180 * PI, axis);
                    orientUnit(rotQuat*dirVec);
                }
            }
            else
                takeOff();
        }
        
        void DemoJet::update(){
            Jet::update();
            if(!onBoard&&!orders.empty()&&orders[0].type==Order::TYPE::ATTACK){
								GameManager *gm = GameManager::getSingleton();
								/*
                ISceneManager *smgr = gm->getDevice()->getSceneManager();
                ISceneNode *collNode = castRay(smgr,pos,pos+dirVec*length);

                if(collNode && collNode != node){
                    InGameAppState *inGameState = ((InGameAppState*)gm->getStateManager()->getAppState(AppStateTypes::IN_GAME_STATE)); 

                    for(Player *p : inGameState->getPlayers())
                        for(Unit *u : p->getUnits())
                            if(u->getNode() == collNode)
                                u->takeDamage(damage);

                    blowUp();
                }
                else if(pos.y<0)
                    blowUp();
										*/
            }
        }
    }
}
