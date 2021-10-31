#include <model.h>

#include "guidedMissile.h"
#include "unit.h"
#include "guidedMissileData.h"
#include "util.h"

using namespace game::core;
using namespace game::util;
using namespace game::content;
using namespace vb01;
using namespace std;

namespace game{
    namespace content{
        GuidedMissile::GuidedMissile(Unit *unit, Vector3 pos, Vector3 target, Vector3 dirVec, Vector3 leftVec, Vector3 upVec, int id, int weaponTypeId, int weaponId) :
        Projectile(unit, nullptr, pos, dirVec, leftVec, upVec, id, weaponTypeId, weaponId) {
            speed = .05;

            for (int i = 0; i < 180; i++)
                arcLength += speed * cos(turnAngle * i);

            this->target = target;
            Vector3 targVec = Vector3(target.x - initPos.x, 0, target.z - initPos.z);
            b = targVec.getLength() / 2;
            x = -b;
            dirVec = Vector3(0,1,0);
            upVec = -targVec.norm();
            leftVec = Quaternion(PI / 2, dirVec) * dirVec;
            rayLength = 3;
            damage = 100;
        }

        void GuidedMissile::update() {
            Projectile::update();
            updateVecs();
            node->setPosition(pos);
            checkForCollision();
        }

        void GuidedMissile::updateVecs() {
            if(x < b){
                Vector3 targVec = Vector3(target.x - initPos.x, 0, target.z - initPos.z);
                pos = pos + targVec.norm() * speed;
                x += speed;
                pos.y = sqrt(1. - x * x / (b * b)) * a;
                float tanAngle = atan(-a*x/(b*b*sqrt(1.-x*x/(b*b))));
                Quaternion rotQuat = Quaternion(tanAngle, leftVec);
                Vector3 dirProj = (dirVec == Vector3::VEC_J ? upVec : Vector3(dirVec.x, 0, dirVec.z).norm());
                orientProjectile(rotQuat * targVec);
            }
            else
                pos.y -= speed;
        }
    }
}
