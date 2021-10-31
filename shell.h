#pragma once
#ifndef SHELL_H
#define SHELL_H

#include "gameManager.h"
#include "projectile.h"

namespace game{
    namespace content {
        class Shell : public Projectile {
        public:
            Shell(Unit*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
            ~Shell(){}
            virtual void update();
        protected:
            s64 initTime;
            virtual void updateVecs(vb01::Vector3, float);
        };
    }
}

#endif
