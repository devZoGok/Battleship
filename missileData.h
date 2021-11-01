#pragma once
#ifndef MISSILEDATA_H
#define MISSILEDATA_H

#include "unitData.h"
#include "projectileData.h"

namespace battleship{
    namespace projectileData{
        const int selfDestructTime[unitData::numberOfUnits][13]{
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

#endif

