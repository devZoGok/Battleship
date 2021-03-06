#pragma once
#ifndef DESTROYER_H
#define DESTROYER_H

#include"Vessel.h"
#include"DestroyerData.h"
#include"Player.h"
#include"util.h"

namespace game
{
    namespace content {
        class Destroyer : public Vessel {
        public:
            Destroyer(core::GameManager*, Player*,vector3df, int);
            void dropDepthCharge();
            void attack(Order);
            void update();
        private:
            void reload();
            inline bool canDropDepthCharge(){return util::getTime()-lastDropTime>rateOfDrops;}
            bool reloading = false;
            float maxDepthChargeDropRange;
            int depthCharges, maxDepthCharges, rateOfDrops, reloadRate;
            s64 lastShotTime=0, reloadStartTime=0,lastDropTime=0;
        };
    }
}

#endif
