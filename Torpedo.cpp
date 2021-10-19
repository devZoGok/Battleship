#include"Torpedo.h"

using namespace game::core;

namespace game{
    namespace content{
        Torpedo::Torpedo(GameManager *gM, Unit *unit, irr::core::vector3df pos, irr::core::vector3df dir, irr::core::vector3df left, irr::core::vector3df up, int id,int weaponTypeId,int weaponId) :
            Projectile(gM, unit, nullptr, pos, dir, left, up, id,weaponTypeId,weaponId) {this->damage=50;}
            
        void Torpedo::update() {
            Projectile::update();
            pos += dirVec*speed;
            node->setPosition(pos);
        }
    }
}
