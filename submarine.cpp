#include <time.h>
#include <node.h>

#include "submarine.h"
#include "defConfigs.h"
#include "torpedo.h"
#include "projectileData.h"

using namespace game::core;
using namespace game::util;
using namespace vb01;

namespace game{
    namespace content{
        Submarine::Submarine(Player *player, Vector3 pos, int id) : Unit(player,pos, id) {}

        void Submarine::attack(Order order) {
            float angle = dirVec.getAngleBetween(*(order.targetPos[0]) - pos);

            if(angle / PI * 180 > 50)
                Unit::move(order, range);

            if (canFire()) {
                Vector3 p = pos + leftVec * projectileData::pos[id][0][0].x + upVec * projectileData::pos[id][0][0].y - dirVec * projectileData::pos[id][0][0].z;
                addProjectile(new Torpedo(this, p, dirVec, leftVec, upVec, getId(), 0, 0));
                lastShotTime=getTime();
            }
        }
        
        void Submarine::emerge() {
            submerged = false;
            pos.y += 5;
            node->setPosition(pos);
        }
        
        void Submarine::submerge() {
            submerged = true;
            pos.y -= 5;
            node->setPosition(pos);
        }
    }
}
