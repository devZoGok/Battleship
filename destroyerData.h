#pragma once
#ifndef DESTROYER_DATA_H
#define DESTROYER_DATA_H

#include "vesselData.h"

namespace battleship{
    namespace unitData {
        const int rateOfDrops[numberOfUnits]{0, 0, 500, 500};
        const int reloadRate[numberOfUnits]{0, 0, 2000, 2000};
        const int maxDepthCharges[numberOfUnits]{0, 0, 15, 15};
        const float maxDepthChargeDropRange[numberOfUnits]{0, 0, 3, 3};
    }
}

#endif
