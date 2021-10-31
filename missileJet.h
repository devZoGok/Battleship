#pragma once
#ifndef MISSILE_JET_H
#define MISSILE_JET_H

#include <util.h>

#include "jet.h"
#include "player.h"
#include "missile.h"

namespace game{
    namespace content {
        class MissileJet : public Jet {
        public:
            MissileJet(Player*, vb01::Vector3, int, bool = 1);
            void installMissiles(bool);
        private:
            void attack(Order);
            void fireMissile(vb01::Vector3*);
            inline bool canFire() {return missiles > 0 && vb01::getTime() - lastFireTime > rateOfFire;}
            MissileType type;
            vb01::Node *missileNodes[2]{nullptr,nullptr};
            bool missilesInstalled = false;
            int missiles = 0, rateOfFire = 2001;
            s64 lastFireTime = 0;
        };
    }
}

#endif
