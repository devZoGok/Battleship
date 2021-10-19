#include"Submarine.h"
#include<time.h>
#include"DefConfigs.h"
#include"Torpedo.h"
#include"ProjectileData.h"

using namespace game::core;
using namespace game::util;

namespace game{
    namespace content{
        Submarine::Submarine(GameManager *gM, Player *player,vector3df pos, int id) : Unit(gM, player,pos, id) {}

        void Submarine::attack(Order order) {
            float angle=getAngleBetween(dirVec,*(order.targetPos[0])-pos);
            if(angle/PI*180>50)
                Unit::move(order, range);
            if (canFire()) {
                vector3df p = pos + projectileData::pos[id][0][0].X * leftVec + projectileData::pos[id][0][0].Y * upVec - projectileData::pos[id][0][0].Z*dirVec;
                addProjectile(new Torpedo(gameManager, this, p, dirVec, leftVec, upVec, getId(), 0, 0));
                lastShotTime=getTime();
            }
        }
        
        void Submarine::emerge() {
            submerged = false;
            pos.Y += 5;
            node->setPosition(pos);
        }
        
        void Submarine::submerge() {
            submerged = true;
            pos.Y -= 5;
            node->setPosition(pos);
        }
    }
}
