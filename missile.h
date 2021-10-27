#pragma once
#ifndef MISSILE_H
#define MISSILE_H

#include "projectile.h"

namespace game{
    namespace content{
        enum MissileType {AAM,AWM};
        
        class Missile : public Projectile {
        public:
            Missile(core::GameManager*, Unit*, irr::scene::ISceneNode*, irr::core::vector3df*,irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            ~Missile(){}
            void update();
        private:
            s64 initTime;
            int selfDistructTime=10000;
            irr::core::vector3df *target;
            float rotationSpeed=5;
            MissileType type;
        };
    }
}

#endif
