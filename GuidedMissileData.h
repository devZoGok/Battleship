#pragma once
#ifndef GUIDED_MISSILE_DATA_H
#define GUIDED_MISSILE_DATA_H

#include"ProjectileData.h"

namespace game
{
    namespace content{
        namespace projectileData {
            const vector3df guidedMissilePos[unitData::numberOfUnits][6]{
                {},
                {},
                {},
                {},
                { vector3df(0.24482, 1.14985, 1.32775), vector3df(0.24482, 1.14985, 1.62698), vector3df(0.24482, 1.14985, 1.88017),
                    vector3df(-0.24482, 1.14985, 1.32775), vector3df(-0.24482, 1.14985, 1.62698), vector3df(-0.24482, 1.14985, 1.88017)},
                { vector3df(0.17192, 0.57858, 0.64471), vector3df(-0.17192, 0.57858, 0.64471)}
            };
        }
    }
}

#endif
