#pragma once
#ifndef MISSILE_JET_H
#define MISSILE_JET_H

#include "jet.h"
#include "player.h"
#include "missile.h"

namespace game{
    namespace content {
        class MissileJet : public Jet {
        public:
            MissileJet(core::GameManager*, Player*,vector3df, int, bool = 1);
            void installMissiles(bool);
        private:
            void attack(Order);
            void fireMissile(vector3df*);
            inline bool canFire() {return missiles>0&&util::getTime() - lastFireTime > rateOfFire;}
            MissileType type;
            irr::scene::ISceneNode *missileNodes[2]{nullptr,nullptr};
            bool missilesInstalled=false;
            int missiles=0,rateOfFire=2001;
            s64 lastFireTime=0;
        };
    }
}

#endif
