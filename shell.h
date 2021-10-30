#pragma once
#ifndef SHELL_H
#define SHELL_H

#include "gameManager.h"
#include "projectile.h"

namespace game{
    namespace content {
        class Shell : public Projectile {
        public:
            Shell(Unit*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            ~Shell(){}
            virtual void update();
        protected:
            s64 initTime;
            virtual void updateVecs(irr::core::vector3df, float);
        };
    }
}

#endif
