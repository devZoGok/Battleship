#include"Shell.h"
#include"DefConfigs.h"
#include"ProjectileData.h"
#include"util.h"
#include"Unit.h"

using namespace game::core;
using namespace game::util;
using namespace irr::core;

namespace game{
    namespace content{
        Shell::Shell(GameManager *gM, Unit *unit, vector3df pos, vector3df dir, vector3df left, vector3df up, int id, int weaponTypeId, int weaponId) : Projectile(gM, unit, nullptr, pos, dir, left, up, id, weaponTypeId, weaponId) {
            this->speed = projectileData::speed[id][weaponTypeId][weaponId];
            node->setScale(vector3df(1, 1, 1) * projectileData::scale[id][weaponTypeId][weaponId]);
            initTime = getTime();
        }

        void Shell::update() {
            //x=v*cos(a)*t
            //y=v*sin(a)*t+0.5(g*t^2)
            Projectile::update();
            vector3df straightVec = vector3df(dirVec.X, 0, dirVec.Z).normalize();
            double time = double(getTime() - initTime)/1000,
                offsetX=speed * cos(angle) * time,
                offsetY=speed * sin(angle) * time - .5 * (g * time * time);
            pos = initPos + straightVec * offsetX + vector3df(0, offsetY, 0);
            node->setPosition(pos);
            updateVecs(straightVec, time);
            checkForCollision();
        }

        void Shell::updateVecs(vector3df straightVec, float time) {
            // f'(t)=dy/dx=(v*sin(a)+g*t)/(v*cos(a))
            float tanAngle = -atan((speed * sin(angle) + g * time) / (speed * cos(angle)));
            quaternion rotQuat = rotQuat.fromAngleAxis( tanAngle, leftVec);
            dirVec = rotQuat*straightVec, upVec = rotQuat * vector3df(0, 1, 0);
            vector3df rotVec=node->getRotation();
            rotVec=vector3df(tanAngle/PI*180,rotVec.Y,rotVec.Z);
            node->setRotation(rotVec);
        }
    }
}
