#include "guidedMissile.h"
#include "unit.h"
#include "guidedMissileData.h"
#include "util.h"

using namespace game::core;
using namespace game::util;
using namespace game::content;
using namespace irr::core;

namespace game{
    namespace content{
        GuidedMissile::GuidedMissile(GameManager *gM, Unit *unit, vector3df pos, vector3df target, vector3df dirVec, vector3df leftVec, vector3df upVec, int id, int weaponTypeId, int weaponId) :
        Projectile(gM, unit, nullptr, pos, dirVec, leftVec, upVec, id, weaponTypeId, weaponId) {
            speed = .05;
            for (int i = 0; i < 180; i++)
                arcLength += speed * cos(turnAngle * i);
            this->target = target;
            vector3df targVec = vector3df(target.X-initPos.X, 0, target.Z-initPos.Z);
            b=targVec.getLength()/2;
            x=-b;
            dirVec=vector3df(0,1,0);
            upVec=-targVec.normalize();
            leftVec=quaternion(0,0,0,1).fromAngleAxis(PI/2,dirVec)*upVec;
            rayLength=3;
            damage=100;
        }

        void GuidedMissile::update() {
            Projectile::update();
            updateVecs();
            node->setPosition(pos);
            checkForCollision();
        }

        void GuidedMissile::updateVecs() {
            if(x<b){
                vector3df targVec = vector3df(target.X-initPos.X, 0, target.Z-initPos.Z);
                pos += targVec.normalize()*speed;
                x += speed;
                pos.Y=sqrt(1.-x*x/(b*b))*a;
                float tanAngle = atan(-a*x/(b*b*sqrt(1.-x*x/(b*b))));
                quaternion rotQuat = quaternion().fromAngleAxis(tanAngle, leftVec);
                vector3df dirProj=dirVec==vector3df(0,1,0)?upVec:vector3df(dirVec.X,0,dirVec.Z).normalize();
                orientProjectile(rotQuat * targVec);
            }
            else
                pos.Y-=speed;
        }
    }
}
