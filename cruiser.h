#pragma once
#ifndef CRUISER_H
#define CRUISER_H

#include "vessel.h"
#include "player.h"

namespace game
{
    namespace content {
        class Cruiser : public Vessel {
        public:
            Cruiser(Player*,vector3df, int);
            void launch(Order);
        private:
            bool canFire();
            int guidedMissiles, reloadTime = 5000;
            s64 lastUpdateTime;
        };
    }
}

#endif
