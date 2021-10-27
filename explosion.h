#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <irrlicht.h>

namespace game{
    namespace core{
        class InGameAppState;
    }
    namespace content{
        void detonate(core::InGameAppState*,irr::core::vector3df,irr::core::vector3df);
        void detonateDepthCharge(core::InGameAppState*,irr::core::vector3df);
        void detonateTorpedo(core::InGameAppState*,irr::core::vector3df);
    }
}

#endif
