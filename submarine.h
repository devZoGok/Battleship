#pragma once
#ifndef SUBMARINE_H
#define SUBMARINE_H

#include <util.h>

#include "unit.h"
#include "player.h"

namespace game{
    namespace content{
        class Submarine : public Unit {
        public:
            Submarine(Player*, vb01::Vector3, int);
            inline bool isSubmerged() {return submerged;}
            void emerge();
            void submerge();
        private:
            void attack(Order);
            inline bool canFire(){return vb01::getTime() - lastShotTime > rateOfFire;}
            bool submerged = false;
            int rateOfFire=2000;
            s64 lastShotTime=0;
        };
    }
}

#endif
