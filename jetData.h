#pragma once
#ifndef JET_DATA_H
#define JET_DATA_H

#include "unitData.h"

/*
0：戦艦
1：駆逐艦
2：巡洋艦
3：航空母艦
4：潜水艦
5: 戦闘機
6：巡航ミサイル
*/

namespace game{
    namespace content {
        namespace unitData {
            const float pitchSpeed[numberOfUnits]{0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5};
        }
    }
}

#endif
