#pragma once
#ifndef PROJECTILE_DATA_H
#define PROJECTILE_DATA_H

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

namespace battleship{
    namespace projectileData {
				using namespace configData;

        const int numberOfTypesOfWeapons = 2;
        const float speed[unitData::numberOfUnits][numberOfTypesOfWeapons][13]{
            {
                {10,10,10,10,10,10,10,10,10,10,10}
            },
            {
                {10,10,10,10,10,10,10,10,10,10,10}
            },
            {
                {10, 10, 10, 10}
            },
            {
                {10, 10, 10, 10}
            },
            {
                {5,5}
            },
            {
                {7,7,7,7,7},
                {}
            },
            {},
            {},
            {.05},
            {
                {.1, .1},
                {.1, .1}
            }
        };
        const float length[unitData::numberOfUnits][numberOfTypesOfWeapons][13]{
            {
                {1,1,1,1,1,1,1,1,1,1,1,1,1}
            },
            {
                {1,1,1,1,1,1,1,1,1,1,1,1,1}
            },
            {
                {1,1,1,1}
            },
            {
                {1,1,1,1}
            },
            {
                {1,1}
            },
            {
                {1,1,1,1,1}
            },
            {},
            {},
            {
                {.5}
            },
            {
                {1,1},
                {1,1}
            },
            {}
        };
        const float scale[unitData::numberOfUnits][numberOfTypesOfWeapons][13]{
            {
                {1, 1, 1, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5}
            },
            {
                {1, 1, 1, .23, .23, .23, .23, .23, .23, .23, .23, .23, .23}
            },
            {
                {.45, .45, .45, .45}
            },
            {
                {.5, .5, .5, .5}
            },
            {
                {.95, .95}
            },
            {
                {.6, .6, .6, .6, .6}
            }
        };
        const vb01::Vector3 pos[unitData::numberOfUnits][numberOfTypesOfWeapons][13]{
            {
                {
										vb01::Vector3(0, 0, 1.05), 
                    vb01::Vector3(0, 0, 1.05), 
                    vb01::Vector3(0, 0, 1.05),
                    vb01::Vector3(0, 0, 1.5), 
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5),
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5), 
                    vb01::Vector3(0, 0, 2.5)
                }
            },
            {
                {
										vb01::Vector3(0, 0, 0.79254), 
                    vb01::Vector3(0, 0, 0.79254), 
                    vb01::Vector3(0, 0, 0.79254),
                    vb01::Vector3(0, 0, 0.17723), 
                    vb01::Vector3(0, 0, 0.17723), 
                    vb01::Vector3(0, 0, 0.17723),
                    vb01::Vector3(0, 0, 0.17723), 
                    vb01::Vector3(0, 0, 0.17723), 
                    vb01::Vector3(0, 0, 0.17723),
                    vb01::Vector3(0, 0, 0.17723), 
                    vb01::Vector3(0, 0, 0.17723), 
                    vb01::Vector3(0, 0, 0.17723),
                    vb01::Vector3(0, 0, 0.17723)
                }
            },
            {
                {
										vb01::Vector3(0, 0, -0.36178), 
                    vb01::Vector3(0, 0, -0.36178), 
                    vb01::Vector3(0, 0, -0.36178), 
                    vb01::Vector3(0, 0, -0.36178)
                },
                {vb01::Vector3(0, -3, 0)}
            },
            {
                {
										vb01::Vector3(0, 0, -0.30074), 
                    vb01::Vector3(0, 0, -0.30074), 
                    vb01::Vector3(0, 0, -0.30074), 
                    vb01::Vector3(0, 0, -0.30074)
                },
                {vb01::Vector3(0, -2, 0)}
            },
            {
                {vb01::Vector3(0, 0, -0.62118), vb01::Vector3(0, 0, -0.62118)},
                {
										vb01::Vector3(0.24482, 1.14985, 1.32775), 
                    vb01::Vector3(0.24482, 1.14985, 1.62698), 
                    vb01::Vector3(0.24482, 1.14985, 1.88017),
                    vb01::Vector3(-0.24482, 1.14985, 1.32775), 
                    vb01::Vector3(-0.24482, 1.14985, 1.62698), 
                    vb01::Vector3(-0.24482, 1.14985, 1.88017)
                }
            },
            {
                {
										vb01::Vector3(0, 0, -0.59117), 
                    vb01::Vector3(0, 0, -0.59117), 
                    vb01::Vector3(0, 0, -0.59117), 
                    vb01::Vector3(0, 0, -0.59117), 
                    vb01::Vector3(0, 0, -0.59117)
                },
                {vb01::Vector3(0.17192, 0.57858, 0.64471), vb01::Vector3(-0.17192, 0.57858, 0.64471)}
            },
            {},
            {},
            {
                {vb01::Vector3(0,-.3,-1.5)}
            },
            {
                {vb01::Vector3(-.25,0,0),vb01::Vector3(.25,0,0)},
                {vb01::Vector3(-.25,0,0),vb01::Vector3(.25,0,0)}
            },
            {}
        };
        const int damage[unitData::numberOfUnits][numberOfTypesOfWeapons][13]{
            {
                {50, 50, 50, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}
            },
            {
                {50, 50, 50, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}
            },
            {
                {50,50,50,50},
                {50}
            },
            {
                {50,50,50,50},
                {50}
            },
            {
                {10,10},
                {}
            },
            {
                {10,10,10,10,10},
                {}
            },
            {},
            {},
            {
                {}
            },
            {
                {50,50},
                {50,50}
            },
            {}
        };
        const std::string name[unitData::numberOfUnits][numberOfTypesOfWeapons]{
            {"shell"},
            {"shell"},
            {"shell"},
            {"shell"},
            {"shell","guidedMissile"},
            {"shell","guidedMissile"},
            {""},
            {""},
            {"torpedo"},
            {"missile","missile"},
        };
        const std::string meshPath[unitData::numberOfUnits][numberOfTypesOfWeapons]{
            {PATH + "Models/Shell/shell.x"},
            {PATH + "Models/Shell/shell.x"},
            {PATH + "Models/Shell/shell.x", PATH + "Models/Depth charge/depthCharge.x"},
            {PATH + "Models/Shell/shell.x", PATH + "Models/Depth charge/depthCharge.x"},
            {PATH + "Models/Shell/shell.x", PATH + "Models/Guided missile/guidedMissile.x"},
            {PATH + "Models/Shell/shell.x", PATH + "Models/Guided missile/guidedMissile.x"},
            {},
            {},
            {PATH + "Models/Torpedo/torpedo.x"},
            {PATH + "Models/Jets/aam.x", PATH + "Models/Jets/awm.x"},
        };
        const std::string diffuseMapTextPath[unitData::numberOfUnits][numberOfTypesOfWeapons]{
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
            {}
        };
    }
}

#endif
