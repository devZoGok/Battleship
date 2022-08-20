#include <algorithm>

#include <stateManager.h>

#include "destroyer.h"
#include "destroyerData.h"
#include "defConfigs.h"
#include "depthCharge.h"
#include "inGameAppState.h"
#include "projectileData.h"
#include "util.h"

using namespace std;
using namespace vb01;

namespace battleship{
	using namespace unitData;

    Destroyer::Destroyer(Player *player, vb01::Vector3 pos,int id) : Vessel(player, pos, id) {
        this->maxDepthCharges = unitData::maxDepthCharges[id];
        depthCharges = maxDepthCharges;
        this->rateOfDrops = unitData::rateOfDrops[id];
        this->reloadRate = unitData::reloadRate[id];
        this->maxDepthChargeDropRange = unitData::maxDepthChargeDropRange[id];
    }

    void Destroyer::update(){
        Vessel::update();
        reload();
    }

    void Destroyer::attack(Order order){
        Vector3 t = order.targets[0].pos;
        bool sub = false;
        InGameAppState *inGameState=((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

        for(Player *p : inGameState->getPlayers())
            for(Unit *u : p->getUnits())
                if(u->getType() == UnitType::SUBMARINE && order.targets[0].pos == u->getPos())
                    sub = true;

        if(sub)
            dropDepthCharge();
        else 
            Vessel::attack(order);
    }

    void Destroyer::dropDepthCharge() {
        if(canDropDepthCharge()){
            float angle = rand() % 360;
            float radius = 1;
            Vector3 offsetVec = Vector3(cos(angle), 0, sin(angle)) * radius;
            addProjectile(new DepthCharge(this, pos + projectileData::pos[id][1][1] + offsetVec, dirVec, leftVec, upVec, id, 1, 0));
            lastDropTime = getTime();

            if(depthCharges == maxDepthCharges)
                reloadStartTime = getTime();
            
        }
    }
    void Destroyer::reload(){
        if(depthCharges < maxDepthCharges){
            reloading = true;

            if(getTime() - reloadStartTime > reloadRate) {
                reloadStartTime += reloadRate;
                depthCharges++;
            }
        }
        else
            reloading = false;
    }
}
