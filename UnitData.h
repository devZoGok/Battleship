#pragma once
#ifndef UNIT_DATA_H
#define UNIT_DATA_H

#include<irrlicht.h>
#include"DefConfigs.h"

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
            using namespace irr::core;
            using namespace irr::io;
            using namespace game::core;
            
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
            const vector3df unitCornerPoints[numberOfUnits][8]{
                {
                    vector3df(.9, -1, -6), 
                    vector3df(-.9, -1, -6), 
                    vector3df(-.9, -1, 5.8), 
                    vector3df(.9, -1, 5.8), 
                    vector3df(.9, 3.5, -6), 
                    vector3df(-.9, 3.5, -6), 
                    vector3df(-.9, 3.5, 5.8), 
                    vector3df(.9, 3.5, 5.8)
                },
                {
                    vector3df(.5, -.5, -7.05), 
                    vector3df(-.5, -.5, -7.05), 
                    vector3df(-.5, -.5, 6.4), 
                    vector3df(.5, -.5, 6.4),
                    vector3df(.5, 3, -7.05), 
                    vector3df(-.5, 3, -7.05), 
                    vector3df(-.5, 3, 6.4), 
                    vector3df(.5, 3, 6.4)
                },
                {
                    vector3df(.5, -.5, -3.4), 
                    vector3df(-.5, -.5, -3.4), 
                    vector3df(-.5, -.5, 3.4),
                    vector3df(.5, -.4, 3.4), 
                    vector3df(.5, 2.1, -3.4), 
                    vector3df(-.5, 2, -3.4),
                    vector3df(-.5, 2, 3.4), 
                    vector3df(.5, 2, 3.4)
                },
                {
                    vector3df(.5, -.25, -4.8), 
                    vector3df(-.5, -.25, -4.8), 
                    vector3df(-.5, -.25, 4.7), 
                    vector3df(.5, -.25, 4.7), 
                    vector3df(.5, .8, -4.8), 
                    vector3df(-.5, .8, -4.8), 
                    vector3df(-.5, .8, 4.7), 
                    vector3df(.5, .8, 4.7)
                },
                {
                    vector3df(.7, -.3, -4), 
                    vector3df(-.7, -.3, -4), 
                    vector3df(-.7, -.3, 3.4), 
                    vector3df(.7, -.3, 3.4), 
                    vector3df(.7, 2.9, -4), 
                    vector3df(-.7, 2.9, -4), 
                    vector3df(-.7, 2.9, 3.4), 
                    vector3df(.7, 2.9, 3.4)
                },
                {
                    vector3df(.5, -.5, -6.1), 
                    vector3df(-.5, -.5, -6.1), 
                    vector3df(-.5, -.5, 5.2), 
                    vector3df(.5, -.5, 5.2), 
                    vector3df(.5, 2.2, -6.1), 
                    vector3df(-.5, 2.2, -6.1), 
                    vector3df(-.5, 2.2, 5.2), 
                    vector3df(.5, 2.2, 5.2)
                },
                {
                    vector3df(2, -.5, -10), 
                    vector3df(-2, -.5, -10), 
                    vector3df(-2, -.5, 9), 
                    vector3df(2, -.5, 9), 
                    vector3df(2, 5.5, -10), 
                    vector3df(-2, 5.5, -10), 
                    vector3df(-2, 5.5, 9), 
                    vector3df(2, 5.5, 9)
                },
                {
                    vector3df(2, -1, -10), 
                    vector3df(-2, -1, -10), 
                    vector3df(-2, -1, 9), 
                    vector3df(2, -1, 9), 
                    vector3df(2, 4.5, -10), 
                    vector3df(-2, 4.5, -10), 
                    vector3df(-2, 4.5, 9), 
                    vector3df(2, 4.5, 9)
                },
                {
                    vector3df(.25, -.5, -2), 
                    vector3df(-.25, -.5, -2), 
                    vector3df(-.25, -.5, 3), 
                    vector3df(.25, -.5, 3), 
                    vector3df(.25, .7, -2), 
                    vector3df(-.25, .7, -2), 
                    vector3df(-.25, .7, 3), 
                    vector3df(.25, .7, 3)
                },
                {
                    vector3df(.6,-.1,-1.3),
                    vector3df(-.6,-.1,-1.3),
                    vector3df(-.6,-.1,.8),
                    vector3df(.6,-.1,.8),
                    vector3df(.6,.3,-1.3),
                    vector3df(-.6,.3,-1.3),
                    vector3df(-.6,.3,.8),
                    vector3df(.6,.3,.8)
                },
                {
                    vector3df(.5,-.1,-1.3),
                    vector3df(-.5,-.1,-1.3),
                    vector3df(-.5,-.1,.7),
                    vector3df(.5,-.1,.7),
                    vector3df(.5,.15,-1.3),
                    vector3df(-.5,.15,-1.3),
                    vector3df(-.5,.15,.7),
                    vector3df(.5,.15,.7)
                }
            };
            const vector3df unitCuboidDimensions[numberOfUnits][8]{
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1), 
                    vector3df(1, 1, 1)
                },
                {
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1)
                },
                {
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1)
                },
                {
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1),
                    vector3df(.1, .1, .1), 
                    vector3df(.1, .1, .1)
                }
            };
            const float unitAxisLength[numberOfUnits]{8, 8,6,6,5, 5,8, 8, 7,2,2};
            const float lineOfSight[numberOfUnits]{5, 8, 4, 4, 3, 3, 6, 6, 3, 3, 8};
            const stringw name[numberOfUnits]{
                "Battleship", "Battleship", 
                "Destroyer", "Destroyer", 
                "Cruiser", "Cruiser", 
                "Aircraft carrier", "Aircraft carrier", 
                "Submarine", 
                "Jet", "Jet"
            };
            const path meshPath[numberOfUnits]{
                "hull00.x", "hull01.x",
                "hull00.x", "hull01.x", 
                "hull00.x", "hull01.x", 
                "hull00.x", "hull01.x",
                "hull.x",
                "jet00.x", "jet01.x"
            };
            const path basePath[numberOfUnits]{
                PATH + "Models/Battleships/", PATH + "Models/Battleships/",
                PATH + "Models/Destroyers/", PATH + "Models/Destroyers/",
                PATH + "Models/Cruisers/", PATH + "Models/Cruisers/",
                PATH + "Models/Aircraft carriers/", PATH + "Models/Aircraft carriers/",
                PATH + "Models/Submarines/",
                PATH + "Models/Jets/", PATH + "Models/Jets/"};
        }
    }
}

#endif
