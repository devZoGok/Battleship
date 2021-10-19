#pragma once
#ifndef SHELL_H
#define SHELL_H

#include"GameManager.h"
#include"Projectile.h"

namespace game{
    namespace content {
        class Shell : public Projectile {
        public:
            Shell(core::GameManager*, Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            ~Shell(){}
            virtual void update();
        protected:
            s64 initTime;
            virtual void updateVecs(irr::core::vector3df, float);
        };
    }
}

#endif
