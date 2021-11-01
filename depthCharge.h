#pragma once
#ifndef DEPTH_CHARGE_H
#define DEPTH_CHARGE_H

#include "projectile.h"

namespace battleship{
    class DepthCharge : public Projectile {
    public:
        DepthCharge(Unit*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
        void update();
    private:
        s64 initTime;
        void explode(vb01::Node*);
        void checkForCollision();
    };
}

#endif
