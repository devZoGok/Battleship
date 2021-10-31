#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <vector.h>

namespace game{
    namespace core{
        class InGameAppState;
    }
    namespace content{
        void detonate(core::InGameAppState*, vb01::Vector3, vb01::Vector3);
        void detonateDepthCharge(core::InGameAppState*, vb01::Vector3);
        void detonateTorpedo(core::InGameAppState*, vb01::Vector3);
    }
}

#endif
