#pragma once
#ifndef TORPEDO_H
#define TORPEDO_H

#include "projectile.h"

namespace battleship{
    class Torpedo : public Projectile {
    public:
        Torpedo(Unit*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
        ~Torpedo(){}
        void update();
        //void explode()
    };
}

#endif
