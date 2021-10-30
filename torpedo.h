#pragma once
#ifndef TORPEDO_H
#define TORPEDO_H

#include "projectile.h"

namespace game{
    namespace content{
        class Torpedo : public Projectile {
        public:
            Torpedo(Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            ~Torpedo(){}
            void update();
//             void explode()
        };
    }
}

#endif
