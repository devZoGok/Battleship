#pragma once
#ifndef DEPTH_CHARGE_H
#define DEPTH_CHARGE_H

#include"Projectile.h"

namespace game{
    namespace content{
        class DepthCharge : public Projectile {
        public:
            DepthCharge(core::GameManager*, Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            void update();
        private:
            s64 initTime;
            void explode(irr::scene::ISceneNode*);
            void checkForCollision();
        };
    }
}

#endif
