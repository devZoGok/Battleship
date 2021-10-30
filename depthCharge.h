#pragma once
#ifndef DEPTH_CHARGE_H
#define DEPTH_CHARGE_H

#include "projectile.h"

namespace game{
    namespace content{
        class DepthCharge : public Projectile {
        public:
            DepthCharge(Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            void update();
        private:
            s64 initTime;
            void explode(irr::scene::ISceneNode*);
            void checkForCollision();
        };
    }
}

#endif
