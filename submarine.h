#pragma once
#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "unit.h"
#include "player.h"

namespace game{
    namespace content{
        class Submarine : public Unit {
        public:
            Submarine(Player*, irr::core::vector3df, int);
            inline bool isSubmerged() {return submerged;}
            void emerge();
            void submerge();
        private:
            void attack(Order);
            inline bool canFire(){return util::getTime() - lastShotTime > rateOfFire;}
            bool submerged = false;
            int rateOfFire=2000;
            s64 lastShotTime=0;
        };
    }
}

#endif
