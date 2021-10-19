#pragma once
#ifndef MISSILEDATA_H
#define MISSILEDATA_H

#include"ProjectileData.h"

namespace unitSpace{
    namespace projectileSpace{
        namespace projectileData{
            const int selfDestructTime[numberOfUnits][13]{
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {10000,10000}
            };
        }
    }
}

#endif

