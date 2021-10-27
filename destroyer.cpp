#include <algorithm>

#include "destroyer.h"
#include "destroyerData.h"
#include "defConfigs.h"
#include "depthCharge.h"
#include "inGameAppState.h"
#include "projectileData.h"
#include "util.h"

using namespace std;
using namespace game::core;
using namespace game::util;
using namespace game::content::unitData;

namespace game{
    namespace content{
        Destroyer::Destroyer(GameManager *gM, Player *player,vector3df pos,int id) :Vessel(gM, player,pos, id) {
            this->maxDepthCharges=unitData::maxDepthCharges[id];
            depthCharges=maxDepthCharges;
            this->rateOfDrops=unitData::rateOfDrops[id];
            this->reloadRate=unitData::reloadRate[id];
            this->maxDepthChargeDropRange=unitData::maxDepthChargeDropRange[id];
        }
        void Destroyer::update(){
            Vessel::update();
            reload();
        }
        void Destroyer::attack(Order order){
            vector3df t=*order.targetPos[0];
            bool sub=false;
            InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE));
            for(Player *p : inGameState->getPlayers())
                for(Unit *u : p->getUnits())
                    if(u->getType()==UNIT_TYPE::SUBMARINE&&order.targetPos[0]==u->getPosPtr())
                        sub=true;
            if(sub)
                dropDepthCharge();
            else 
                Vessel::attack(order);
        }
        void Destroyer::dropDepthCharge() {
            if(canDropDepthCharge()){
                float angle=rand()%360;
                float radius=1;
                vector3df offsetVec=vector3df(cos(angle),0,sin(angle))*radius;
                addProjectile(new DepthCharge(gameManager,this,pos+projectileData::pos[id][1][1]+offsetVec,dirVec,leftVec,upVec,id,1,0));
                lastDropTime=getTime();
                if(depthCharges==maxDepthCharges)
                    reloadStartTime=getTime();
                
            }
        }
        void Destroyer::reload(){
            if(depthCharges<maxDepthCharges){
                reloading=true;
                if(getTime()-reloadStartTime>reloadRate) {
                    reloadStartTime+=reloadRate;
                    depthCharges++;
                }
            }
            else
                reloading=false;
        }
    }
}
