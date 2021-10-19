#pragma once
#ifndef CRUISER_H
#define CRUISER_H

#include"Vessel.h"
#include"Player.h"

namespace game
{
    namespace content {
        class Cruiser : public Vessel {
        public:
            Cruiser(core::GameManager*, Player*,vector3df, int);
            void launch(Order);
        private:
            bool canFire();
            int guidedMissiles, reloadTime = 5000;
            s64 lastUpdateTime;
        };
    }
}

#endif
