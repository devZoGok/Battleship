#include <util.h>
#include <model.h>

#include "missile.h"

using namespace game::core;
using namespace game::util;
using namespace vb01;

namespace game{
    namespace content{
        Missile::Missile(Unit *unit, Node *node, Vector3 *target, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) :
            Projectile(unit, node, pos, dir, left, up, id, weaponTypeId, weaponId) {
            this->target = target;
            this->type = (MissileType)weaponId;
            this->initTime = getTime();
        }
        
        void Missile::update() {
            Projectile::update();
            pos = pos + dirVec * speed;
            node->setPosition(pos);
            float angle = dirVec.getAngleBetween(*target - pos);
            Vector3 axis = dirVec.cross(*target - pos).norm();

            if(rotationSpeed / 180 * PI < angle){
                Quaternion rotQuat = Quaternion(rotationSpeed / 180 * PI, axis);
                orientProjectile(rotQuat * dirVec);
            }

            if(!exploded && getTime() - initTime > selfDistructTime)
                Projectile::explode(nullptr);
        }
    }
}
