#include"DepthCharge.h"
#include"InGameAppState.h"
#include"util.h"

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::scene;

namespace game{
    namespace content{
        DepthCharge::DepthCharge(GameManager *gM, Unit *unit, vector3df pos,vector3df dir, vector3df left, vector3df up, int id,int weaponTypeId,int weaponId) :
            Projectile(gM, unit, nullptr, pos, dir, left, up, id,weaponTypeId,weaponId) {
            speed=0;
            initTime=getTime();
        }
        void DepthCharge::update(){
            //x=x0+vX+a*t*t/2
            //y=y0+vY-g*t*t/2
            float t = float(getTime() - initTime) / 1000;
            pos = initPos + dirVec * speed + vector3df(0, speed * t - .5 * g * t * t, 0);
            node->setPosition(pos);
            checkForCollision();
        }
        void DepthCharge::checkForCollision() {
            InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE));
            ISceneNode *collNode=nullptr;
            bool detonated=false;
            if(pos.Y<-10)
                detonated=true;
            if(!detonated)
                for(Player *p : inGameState->getPlayers())
                    for(Unit *u : p->getUnits()){
                        if(u==unit) continue;
                        vector3df p0=u->getCorner(0);
                        vector3df p1=u->getCorner(1);
                        vector3df p3=u->getCorner(3);
                        vector3df p4=u->getCorner(4);
                        if(isWithinCuboid(p0,p1,p3,p4,pos)){
                            detonated=true;
                            collNode=u->getNode();
                        }
                    }
            if(detonated)
                explode(collNode);
        }
        void DepthCharge::explode(ISceneNode *collNode) {
            Projectile::explode(collNode);
        }
    }
}
