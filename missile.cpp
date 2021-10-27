#include "missile.h"

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::scene;

namespace game{
    namespace content{
        Missile::Missile(GameManager *gM, Unit *unit, ISceneNode *node, vector3df *target,vector3df pos,vector3df dir, vector3df left, vector3df up,int id,int weaponTypeId,int weaponId) :
            Projectile(gM, unit, node, pos, dir, left, up,id,weaponTypeId,weaponId) {
            this->target=target;
            this->type=(MissileType)weaponId;
            this->initTime=getTime();
        }
        
        void Missile::update() {
            Projectile::update();
            pos+=dirVec*speed;
            node->setPosition(pos);
            float angle=getAngleBetween(dirVec,*target-pos);
            vector3df axis=dirVec.crossProduct(*target-pos).normalize();
            if(rotationSpeed/180*PI<angle){
                quaternion rotQuat=quaternion().fromAngleAxis(rotationSpeed/180*PI,axis);
                orientProjectile(rotQuat*dirVec);
            }
            if(!exploded&&getTime()-initTime>selfDistructTime)
                Projectile::explode(nullptr);
        }
    }
}
