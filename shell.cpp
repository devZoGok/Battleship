#include <model.h>

#include "shell.h"
#include "defConfigs.h"
#include "projectileData.h"
#include "util.h"
#include "unit.h"

using namespace vb01;

namespace battleship{
    Shell::Shell(Unit *unit, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) : Projectile(unit, nullptr, pos, dir, left, up, id, weaponTypeId, weaponId) {
        this->speed = projectileData::speed[id][weaponTypeId][weaponId];
        node->setScale(Vector3(1, 1, 1) * projectileData::scale[id][weaponTypeId][weaponId]);
        initTime = getTime();
    }

    void Shell::update() {
        //x=v*cos(a)*t
        //y=v*sin(a)*t+0.5(g*t^2)
        Projectile::update();
        Vector3 straightVec = Vector3(dirVec.x, 0, dirVec.z).norm();
        double time = double(getTime() - initTime) / 1000,
            offsetX = speed * cos(angle) * time,
            offsetY = speed * sin(angle) * time - .5 * (g * time * time);
        pos = initPos + straightVec * offsetX + Vector3(0, offsetY, 0);
        node->setPosition(pos);
        updateVecs(straightVec, time);
        checkForCollision();
    }

    void Shell::updateVecs(Vector3 straightVec, float time) {
        // f'(t)=dy/dx=(v*sin(a)+g*t)/(v*cos(a))
        float tanAngle = -atan((speed * sin(angle) + g * time) / (speed * cos(angle)));
        Quaternion rotQuat = Quaternion(tanAngle, leftVec);
        dirVec = rotQuat*straightVec, upVec = rotQuat * Vector3(0, 1, 0);
				/*
        Vector3 rotVec = node->getOrientation();
        rotVec=vector3df(tanAngle/PI*180,rotVec.Y,rotVec.Z);
        node->setRotation(rotVec);
				*/
    }
}
