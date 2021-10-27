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

namespace game{
    namespace content{
        namespace projectileData {
            using namespace irr;
            using namespace irr::core;
            using namespace game::core;
            using namespace irr::io;
            
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
            const vector3df pos[unitData::numberOfUnits][numberOfTypesOfWeapons][13]{
                {
                    {
                        vector3df(0, 0, 1.05), 
                        vector3df(0, 0, 1.05), 
                        vector3df(0, 0, 1.05),
                        vector3df(0, 0, 1.5), 
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5),
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5), 
                        vector3df(0, 0, 2.5)
                    }
                },
                {
                    {
                        vector3df(0, 0, 0.79254), 
                        vector3df(0, 0, 0.79254), 
                        vector3df(0, 0, 0.79254),
                        vector3df(0, 0, 0.17723), 
                        vector3df(0, 0, 0.17723), 
                        vector3df(0, 0, 0.17723),
                        vector3df(0, 0, 0.17723), 
                        vector3df(0, 0, 0.17723), 
                        vector3df(0, 0, 0.17723),
                        vector3df(0, 0, 0.17723), 
                        vector3df(0, 0, 0.17723), 
                        vector3df(0, 0, 0.17723),
                        vector3df(0, 0, 0.17723)
                    }
                },
                {
                    {
                        vector3df(0, 0, -0.36178), 
                        vector3df(0, 0, -0.36178), 
                        vector3df(0, 0, -0.36178), 
                        vector3df(0, 0, -0.36178)
                    },
                    {vector3df(0, -3, 0)}
                },
                {
                    {
                        vector3df(0, 0, -0.30074), 
                        vector3df(0, 0, -0.30074), 
                        vector3df(0, 0, -0.30074), 
                        vector3df(0, 0, -0.30074)
                    },
                    {vector3df(0, -2, 0)}
                },
                {
                    {vector3df(0, 0, -0.62118), vector3df(0, 0, -0.62118)},
                    {
                        vector3df(0.24482, 1.14985, 1.32775), 
                        vector3df(0.24482, 1.14985, 1.62698), 
                        vector3df(0.24482, 1.14985, 1.88017),
                        vector3df(-0.24482, 1.14985, 1.32775), 
                        vector3df(-0.24482, 1.14985, 1.62698), 
                        vector3df(-0.24482, 1.14985, 1.88017)
                    }
                },
                {
                    {
                        vector3df(0, 0, -0.59117), 
                        vector3df(0, 0, -0.59117), 
                        vector3df(0, 0, -0.59117), 
                        vector3df(0, 0, -0.59117), 
                        vector3df(0, 0, -0.59117)
                    },
                    {vector3df(0.17192, 0.57858, 0.64471), vector3df(-0.17192, 0.57858, 0.64471)}
                },
                {},
                {},
                {
                    {vector3df(0,-.3,-1.5)}
                },
                {
                    {vector3df(-.25,0,0),vector3df(.25,0,0)},
                    {vector3df(-.25,0,0),vector3df(.25,0,0)}
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
            const stringw name[unitData::numberOfUnits][numberOfTypesOfWeapons]{
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
            const path meshPath[unitData::numberOfUnits][numberOfTypesOfWeapons]{
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
            const path diffuseMapTextPath[unitData::numberOfUnits][numberOfTypesOfWeapons]{
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
}

#endif
