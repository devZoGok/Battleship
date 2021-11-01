#include <model.h>

#include "torpedo.h"

using namespace vb01;

namespace battleship{
    Torpedo::Torpedo(Unit *unit, Vector3 pos, Vector3 dir, Vector3 left, Vector3 up, int id, int weaponTypeId, int weaponId) :
        Projectile(unit, nullptr, pos, dir, left, up, id,weaponTypeId,weaponId) {this->damage = 50;}
        
    void Torpedo::update() {
        Projectile::update();
        pos = pos + dirVec * speed;
        node->setPosition(pos);
    }
}
