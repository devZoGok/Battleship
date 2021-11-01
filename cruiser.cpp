#include <quaternion.h>

#include "cruiser.h"
#include "cruiserData.h"
#include "guidedMissile.h"
#include "util.h"
#include "projectileData.h"

using namespace vb01;

namespace battleship{
    Cruiser::Cruiser(Player *player, Vector3 pos, int id) : Vessel(player, pos, id) {
        guidedMissiles = unitData::maxGuidedMissiles[id];
    }

    void Cruiser::launch(Order order) {
        if (guidedMissiles > 0) {
            float angle = order.targetPos[0]->getAngleBetween(dirVec);
            Quaternion rotQuat = Quaternion(dirVec.x < 0 ? angle : -angle, Vector3(0, 1, 0));
            Vector3 basePos = leftVec * projectileData::pos[getId()][1][guidedMissiles - 1].x + upVec * projectileData::pos[getId()][1][guidedMissiles - 1].y - dirVec * projectileData::pos[getId()][1][guidedMissiles - 1].z;
            addProjectile(new GuidedMissile(this, pos + basePos, *order.targetPos[0], rotQuat * Vector3(0, 1, 0), rotQuat * Vector3(1, 0, 0), rotQuat * Vector3(0, 0, -1), getId(), 1, 0));
            removeOrder(0);
               guidedMissiles--;
        }
    }
}
