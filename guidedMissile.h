#pragma once
#ifndef GUIDED_MISSILE_H
#define GUIDED_MISSILE_H

#include "projectile.h"

namespace game{
    namespace content{
        class GuidedMissile : public Projectile {
        public:
            GuidedMissile(Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            ~GuidedMissile(){}
            void update();
        private:
            void updateVecs();
            bool firstPhase = true;
            float a = 4,b,x,turnAngle = 1,arcLength = 0.,maxHeight = 5;
            irr::core::vector3df target;
        };
    }
}

#endif
