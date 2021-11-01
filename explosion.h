#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <vector.h>

namespace battleship{
    class InGameAppState;

    void detonate(InGameAppState*, vb01::Vector3, vb01::Vector3);
    void detonateDepthCharge(InGameAppState*, vb01::Vector3);
    void detonateTorpedo(InGameAppState*, vb01::Vector3);
}

#endif
