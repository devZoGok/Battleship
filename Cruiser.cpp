#include"Cruiser.h"
#include"CruiserData.h"
#include"GuidedMissile.h"
#include"util.h"
#include"ProjectileData.h"

using namespace game::core;
using namespace irr::core;
using namespace game::util;

namespace game{
    namespace content{
        Cruiser::Cruiser(GameManager *gM, Player *player, vector3df pos, int id) : Vessel(gM, player, pos, id) {
            guidedMissiles = unitData::maxGuidedMissiles[id];
        }

        void Cruiser::launch(Order order) {
            if (guidedMissiles > 0) {
                float angle = getAngleBetween(*order.targetPos[0], dirVec);
                quaternion rotQuat = rotQuat.fromAngleAxis(dirVec.X < 0 ? angle : -angle, vector3df(0, 1, 0));
                vector3df basePos = leftVec * projectileData::pos[getId()][1][guidedMissiles - 1].X + upVec * projectileData::pos[getId()][1][guidedMissiles - 1].Y - dirVec * projectileData::pos[getId()][1][guidedMissiles - 1].Z;
                addProjectile(new GuidedMissile(gameManager, this, pos + basePos, *order.targetPos[0], rotQuat * vector3df(0, 1, 0), rotQuat * vector3df(1, 0, 0), rotQuat * vector3df(0, 0, -1), getId(), 1, 0));
                removeOrder(0);
//                 guidedMissiles--;
            }
        }
    }
}
