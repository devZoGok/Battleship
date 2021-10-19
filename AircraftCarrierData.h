#pragma once
#ifndef AIRCRAFT_CARRIER_DATA_H
#define AIRCRAFT_CARRIER_DATA_H

#include"UnitData.h"

/*
0：戦艦
1：駆逐艦
2：巡洋艦
3：航空母艦
4：潜水艦
5: 戦闘機
*/

namespace game{
    namespace content {
        namespace unitData {
            const int numJets[numberOfUnits]{0, 0, 0, 0, 0, 0, 6, 6};
            const vector3df jetPos[numberOfUnits][20]{
                {},
                {},
                {},
                {},
                {},
                {},
                {
                    vector3df(0.13674, 1.60034, 7.78832), 
                    vector3df(1.5889, 1.60034, 7.78832), 
                    vector3df(0.13674, 1.60034, 5.47672),
                    vector3df(1.5889, 1.60034, 5.47672), 
                    vector3df(0.13674, 1.60034, 3.37257), 
                    vector3df(1.5889, 1.60034, 3.37257)
                },
                {
                    vector3df(-0.61651, 2.78469, 7.88753), 
                    vector3df(0.40384, 2.78469, 7.88753),
                    vector3df(-0.61651, 2.78469, 5.82783),
                    vector3df(0.40384, 2.78469, 5.82783), 
                    vector3df(-0.61651, 2.78469, 3.87186), 
                    vector3df(0.40384, 2.78469, 3.87186)
                }
            };
            const float runwayLenght[numberOfUnits]{0, 0, 0, 0, 0, 0, 10, 10};
        }
    }
}

#endif
