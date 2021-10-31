#pragma once
#ifndef GUIDED_MISSILE_H
#define GUIDED_MISSILE_H

#include "projectile.h"

namespace game{
    namespace content{
        class GuidedMissile : public Projectile {
        public:
            GuidedMissile(Unit*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
            ~GuidedMissile(){}
            void update();
        private:
            void updateVecs();
            bool firstPhase = true;
            float a = 4,b,x,turnAngle = 1,arcLength = 0.,maxHeight = 5;
            vb01::Vector3 target;
        };
    }
}

#endif
