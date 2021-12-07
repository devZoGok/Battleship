#include <model.h>

#include <stateManager.h>

#include "depthCharge.h"
#include "inGameAppState.h"
#include "util.h"

using namespace vb01;

namespace battleship{
    DepthCharge::DepthCharge(Unit *unit, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) :
        Projectile(unit, nullptr, pos, dir, left, up, id, weaponTypeId, weaponId) {
        speed = 0;
        initTime = getTime();
    }
    void DepthCharge::update(){
        //x=x0+vX+a*t*t/2
        //y=y0+vY-g*t*t/2
        float t = float(getTime() - initTime) / 1000;
        pos = initPos + dirVec * speed + Vector3(0, speed * t - .5 * g * t * t, 0);
        node->setPosition(pos);
        checkForCollision();
    }
    void DepthCharge::checkForCollision() {
        InGameAppState *inGameState = ((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));
        Node *collNode = nullptr;
        bool detonated = false;

        if(pos.y < -10)
            detonated = true;

        if(!detonated)
            for(Player *p : inGameState->getPlayers())
                for(Unit *u : p->getUnits()){
                    if(u == unit) continue;

                    Vector3 p0 = u->getCorner(0);
                    Vector3 p1 = u->getCorner(1);
                    Vector3 p3 = u->getCorner(3);
                    Vector3 p4 = u->getCorner(4);

										/*
                    if(isWithinCuboid(p0,p1,p3,p4,pos)){
                        detonated=true;
                        collNode=u->getNode();
                    }
										*/
                }

        if(detonated)
            explode(collNode);
    }
    void DepthCharge::explode(Node *collNode) {
        Projectile::explode(collNode);
    }
}
