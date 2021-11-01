#pragma once
#ifndef GUIDED_MISSILE_DATA_H
#define GUIDED_MISSILE_DATA_H

#include "projectileData.h"

namespace battleship{
    namespace projectileData {
        const vb01::Vector3 guidedMissilePos[unitData::numberOfUnits][6]{
            {},
            {},
            {},
            {},
            {
								vb01::Vector3(0.24482, 1.14985, 1.32775),
								vb01::Vector3(0.24482, 1.14985, 1.62698),
							 	vb01::Vector3(0.24482, 1.14985, 1.88017),
                vb01::Vector3(-0.24482, 1.14985, 1.32775),
							 	vb01::Vector3(-0.24482, 1.14985, 1.62698),
							 	vb01::Vector3(-0.24482, 1.14985, 1.88017)
						},
            {vb01::Vector3(0.17192, 0.57858, 0.64471), vb01::Vector3(-0.17192, 0.57858, 0.64471)}
        };
    }
}

#endif
