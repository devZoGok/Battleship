#include "demoJet.h"
#include "inGameAppState.h"
#include "util.h"

using namespace game::core;
using namespace game::util;

namespace game{
    namespace content{
        DemoJet::DemoJet(GameManager *gM, Player *player,vector3df pos, int id, bool onBoard) :Jet(gM, player,pos, id,onBoard) {}
        
        void DemoJet::attack(Order order){
            if(!onBoard){
                Unit::attack(order);
                vector3df target=*order.targetPos[0];
                float angle=getAngleBetween(dirVec,target-pos);
                vector3df axis=dirVec.crossProduct(target-pos);
                if(maxTurnAngle<angle/PI*180){
                    quaternion rotQuat=quaternion(0,0,0,0).fromAngleAxis(maxTurnAngle/180*PI,axis);
                    orientUnit(rotQuat*dirVec);
                }
            }
            else
                takeOff();
        }
        
        void DemoJet::update(){
            Jet::update();
            if(!onBoard&&!orders.empty()&&orders[0].type==Order::TYPE::ATTACK){
                ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
                ISceneNode *collNode = castRay(smgr,pos,pos+dirVec*length);
                if(collNode&&collNode!=node){
                    InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE)); 
                    for(Player *p : inGameState->getPlayers())
                        for(Unit *u : p->getUnits())
                            if(u->getNode()==collNode)
                                u->takeDamage(damage);
                    blowUp();
                }
                else if(pos.Y<0)
                    blowUp();
            }
        }
    }
}
