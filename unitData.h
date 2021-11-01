#pragma once
#ifndef UNIT_DATA_H
#define UNIT_DATA_H

#include <vector.h>

#include "defConfigs.h"

/*
0：戦艦
1：駆逐艦
2：巡洋艦
3：航空母艦
4：潜水艦
5: 戦闘機
*/


namespace battleship {
    namespace unitData {
				using namespace configData;

        const int numberOfUnits = 11;
        enum class UNIT_TYPE {VESSEL, DESTROYER, CRUISER, AIRCRAFT_CARRIER, SUBMARINE, MISSILE_JET, DEMO_JET};
        const UNIT_TYPE unitType[numberOfUnits]{
            UNIT_TYPE::VESSEL, UNIT_TYPE::VESSEL,
            UNIT_TYPE::DESTROYER, UNIT_TYPE::DESTROYER,
            UNIT_TYPE::CRUISER, UNIT_TYPE::CRUISER,
            UNIT_TYPE::AIRCRAFT_CARRIER, UNIT_TYPE::AIRCRAFT_CARRIER,
            UNIT_TYPE::SUBMARINE,
            UNIT_TYPE::MISSILE_JET, UNIT_TYPE::DEMO_JET
        };
        const int health[numberOfUnits]{500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
        const float speed[numberOfUnits]{.025, .025, .05, .05, .1, .1, .1, .1, .1, .1, .15};
        const float destinationOffset[numberOfUnits]{.75, .75, 1, 1, .1, .1, .1, .1, .1, .5, .5};
        const float anglePrecision[numberOfUnits]{.1, .1, .1, .1, .1, .1, .1, .1, .1, 3,3};
        const int cost[numberOfUnits]{500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
        const float maxTurnAngle[numberOfUnits]{1, 1, .5, .5, 1, 1, .5, .5, 1, 2, 2};
        const float range[numberOfUnits]{15, 15, 14, 14, 13, 13, 12, 12,15,25};
        const vb01::Vector3 unitCornerPoints[numberOfUnits][8]{
            {
								vb01::Vector3(.9, -1, -6), 
                vb01::Vector3(-.9, -1, -6), 
                vb01::Vector3(-.9, -1, 5.8), 
                vb01::Vector3(.9, -1, 5.8), 
                vb01::Vector3(.9, 3.5, -6), 
                vb01::Vector3(-.9, 3.5, -6), 
                vb01::Vector3(-.9, 3.5, 5.8), 
                vb01::Vector3(.9, 3.5, 5.8)
            },
            {
                vb01::Vector3(.5, -.5, -7.05), 
                vb01::Vector3(-.5, -.5, -7.05), 
                vb01::Vector3(-.5, -.5, 6.4), 
                vb01::Vector3(.5, -.5, 6.4),
                vb01::Vector3(.5, 3, -7.05), 
                vb01::Vector3(-.5, 3, -7.05), 
                vb01::Vector3(-.5, 3, 6.4), 
                vb01::Vector3(.5, 3, 6.4)
            },
            {
                vb01::Vector3(.5, -.5, -3.4), 
                vb01::Vector3(-.5, -.5, -3.4), 
                vb01::Vector3(-.5, -.5, 3.4),
                vb01::Vector3(.5, -.4, 3.4), 
                vb01::Vector3(.5, 2.1, -3.4), 
                vb01::Vector3(-.5, 2, -3.4),
                vb01::Vector3(-.5, 2, 3.4), 
                vb01::Vector3(.5, 2, 3.4)
            },
            {
                vb01::Vector3(.5, -.25, -4.8), 
                vb01::Vector3(-.5, -.25, -4.8), 
                vb01::Vector3(-.5, -.25, 4.7), 
                vb01::Vector3(.5, -.25, 4.7), 
                vb01::Vector3(.5, .8, -4.8), 
                vb01::Vector3(-.5, .8, -4.8), 
                vb01::Vector3(-.5, .8, 4.7), 
                vb01::Vector3(.5, .8, 4.7)
            },
            {
                vb01::Vector3(.7, -.3, -4), 
                vb01::Vector3(-.7, -.3, -4), 
                vb01::Vector3(-.7, -.3, 3.4), 
                vb01::Vector3(.7, -.3, 3.4), 
                vb01::Vector3(.7, 2.9, -4), 
                vb01::Vector3(-.7, 2.9, -4), 
                vb01::Vector3(-.7, 2.9, 3.4), 
                vb01::Vector3(.7, 2.9, 3.4)
            },
            {
                vb01::Vector3(.5, -.5, -6.1), 
                vb01::Vector3(-.5, -.5, -6.1), 
                vb01::Vector3(-.5, -.5, 5.2), 
                vb01::Vector3(.5, -.5, 5.2), 
                vb01::Vector3(.5, 2.2, -6.1), 
                vb01::Vector3(-.5, 2.2, -6.1), 
                vb01::Vector3(-.5, 2.2, 5.2), 
                vb01::Vector3(.5, 2.2, 5.2)
            },
            {
                vb01::Vector3(2, -.5, -10), 
                vb01::Vector3(-2, -.5, -10), 
                vb01::Vector3(-2, -.5, 9), 
                vb01::Vector3(2, -.5, 9), 
                vb01::Vector3(2, 5.5, -10), 
                vb01::Vector3(-2, 5.5, -10), 
                vb01::Vector3(-2, 5.5, 9), 
                vb01::Vector3(2, 5.5, 9)
            },
            {
                vb01::Vector3(2, -1, -10), 
                vb01::Vector3(-2, -1, -10), 
                vb01::Vector3(-2, -1, 9), 
                vb01::Vector3(2, -1, 9), 
                vb01::Vector3(2, 4.5, -10), 
                vb01::Vector3(-2, 4.5, -10), 
                vb01::Vector3(-2, 4.5, 9), 
                vb01::Vector3(2, 4.5, 9)
            },
            {
                vb01::Vector3(.25, -.5, -2), 
                vb01::Vector3(-.25, -.5, -2), 
                vb01::Vector3(-.25, -.5, 3), 
                vb01::Vector3(.25, -.5, 3), 
                vb01::Vector3(.25, .7, -2), 
                vb01::Vector3(-.25, .7, -2), 
                vb01::Vector3(-.25, .7, 3), 
                vb01::Vector3(.25, .7, 3)
            },
            {
                vb01::Vector3(.6,-.1,-1.3),
                vb01::Vector3(-.6,-.1,-1.3),
                vb01::Vector3(-.6,-.1,.8),
                vb01::Vector3(.6,-.1,.8),
                vb01::Vector3(.6,.3,-1.3),
                vb01::Vector3(-.6,.3,-1.3),
                vb01::Vector3(-.6,.3,.8),
                vb01::Vector3(.6,.3,.8)
            },
            {
                vb01::Vector3(.5,-.1,-1.3),
                vb01::Vector3(-.5,-.1,-1.3),
                vb01::Vector3(-.5,-.1,.7),
                vb01::Vector3(.5,-.1,.7),
                vb01::Vector3(.5,.15,-1.3),
                vb01::Vector3(-.5,.15,-1.3),
                vb01::Vector3(-.5,.15,.7),
                vb01::Vector3(.5,.15,.7)
            }
        };
        const vb01::Vector3 unitCuboidDimensions[numberOfUnits][8]{
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1), 
                vb01::Vector3(1, 1, 1)
            },
            {
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1)
            },
            {
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1)
            },
            {
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1),
                vb01::Vector3(.1, .1, .1), 
                vb01::Vector3(.1, .1, .1)
            }
        };
        const float unitAxisLength[numberOfUnits]{8, 8,6,6,5, 5,8, 8, 7,2,2};
        const float lineOfSight[numberOfUnits]{5, 8, 4, 4, 3, 3, 6, 6, 3, 3, 8};
        const std::string name[numberOfUnits]{
            "Battleship", "Battleship", 
            "Destroyer", "Destroyer", 
            "Cruiser", "Cruiser", 
            "Aircraft carrier", "Aircraft carrier", 
            "Submarine", 
            "Jet", "Jet"
        };
        const std::string meshPath[numberOfUnits]{
            "hull00.x", "hull01.x",
            "hull00.x", "hull01.x", 
            "hull00.x", "hull01.x", 
            "hull00.x", "hull01.x",
            "hull.x",
            "jet00.x", "jet01.x"
        };
        const std::string basePath[numberOfUnits]{
            PATH + "Models/Battleships/", PATH + "Models/Battleships/",
            PATH + "Models/Destroyers/", PATH + "Models/Destroyers/",
            PATH + "Models/Cruisers/", PATH + "Models/Cruisers/",
            PATH + "Models/Aircraft carriers/", PATH + "Models/Aircraft carriers/",
            PATH + "Models/Submarines/",
            PATH + "Models/Jets/", PATH + "Models/Jets/"};
    }
}

#endif
