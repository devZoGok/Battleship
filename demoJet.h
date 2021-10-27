#pragma once
#ifndef DEMO_JET_H
#define DEMO_JET_H

#include "jet.h"
#include "player.h"

namespace game{
    namespace content {
        class DemoJet : public Jet {
        public:
            DemoJet(core::GameManager*, Player*,vector3df, int, bool = 1);
            void attack(Order);
            void update();
        private:
            float length=1;
            int damage=250;
        };
    }
}

#endif
