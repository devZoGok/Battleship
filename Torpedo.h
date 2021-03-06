#pragma once
#ifndef TORPEDO_H
#define TORPEDO_H

#include"Projectile.h"

namespace game{
    namespace content{
        class Torpedo : public Projectile {
        public:
            Torpedo(core::GameManager*, Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            ~Torpedo(){}
            void update();
//             void explode()
        };
    }
}

#endif
