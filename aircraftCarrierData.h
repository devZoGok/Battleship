#pragma once
#ifndef AIRCRAFT_CARRIER_DATA_H
#define AIRCRAFT_CARRIER_DATA_H

#include "unitData.h"

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
            const vb01::Vector3 jetPos[numberOfUnits][20]{
                {},
                {},
                {},
                {},
                {},
                {},
                {
										vb01::Vector3(0.13674, 1.60034, 7.78832), 
                    vb01::Vector3(1.5889, 1.60034, 7.78832), 
                    vb01::Vector3(0.13674, 1.60034, 5.47672),
                    vb01::Vector3(1.5889, 1.60034, 5.47672), 
                    vb01::Vector3(0.13674, 1.60034, 3.37257), 
                    vb01::Vector3(1.5889, 1.60034, 3.37257)
                },
                {
										vb01::Vector3(-0.61651, 2.78469, 7.88753), 
                    vb01::Vector3(0.40384, 2.78469, 7.88753),
                    vb01::Vector3(-0.61651, 2.78469, 5.82783),
                    vb01::Vector3(0.40384, 2.78469, 5.82783), 
                    vb01::Vector3(-0.61651, 2.78469, 3.87186), 
                    vb01::Vector3(0.40384, 2.78469, 3.87186)
                }
            };
            const float runwayLenght[numberOfUnits]{0, 0, 0, 0, 0, 0, 10, 10};
        }
    }
}

#endif
