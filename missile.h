#pragma once
#ifndef MISSILE_H
#define MISSILE_H

#include "projectile.h"

namespace battleship{
    enum MissileType {AAM, AWM};
    
    class Missile : public Projectile {
    public:
        Missile(Unit*, vb01::Node*, vb01::Vector3*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
        ~Missile(){}
        void update();
    private:
        s64 initTime;
        int selfDistructTime = 10000;
        vb01::Vector3 *target;
        float rotationSpeed = 5;
        MissileType type;
    };
}

#endif
