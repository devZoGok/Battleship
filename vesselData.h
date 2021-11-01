#pragma once
#ifndef VESSEL_DATA_H
#define VESSEL_DATA_H

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
    namespace unitData {
        enum InitDir{FORWARD,BACKWARD,LEFT,RIGHT};
        const int numberOfTurrets[numberOfUnits]{13, 13, 4, 4, 2, 5};
        const int maxNumTurrets=13;
        const InitDir initDirs[numberOfUnits][maxNumTurrets]{
            {FORWARD,FORWARD,BACKWARD,LEFT,LEFT,LEFT,LEFT,LEFT,RIGHT,RIGHT,RIGHT,RIGHT,RIGHT},
            {FORWARD,FORWARD,BACKWARD,LEFT,LEFT,LEFT,LEFT,LEFT,RIGHT,RIGHT,RIGHT,RIGHT,RIGHT},
            {FORWARD,FORWARD,BACKWARD,BACKWARD},
            {FORWARD,FORWARD,BACKWARD,BACKWARD},
            {FORWARD,BACKWARD},
            {FORWARD,FORWARD,FORWARD,BACKWARD,BACKWARD},
        };
        const std::string turretNames[numberOfUnits][maxNumTurrets]{
            {
                "bigTurret00", 
                "bigTurret00", 
                "bigTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00", 
                "smallTurret00"
            },
            {
                "bigTurret01", 
                "bigTurret01", 
                "bigTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01", 
                "smallTurret01"
            },
            {"turret00", "turret00", "turret00", "turret00"},
            {"turret01", "turret01", "turret01", "turret01"},
            {"turret00", "turret00"},
            {"turret01", "turret01", "turret01", "turret01", "turret01"}
        };
        const vb01::Vector3 turretPos[numberOfUnits][maxNumTurrets]{
            {
								vb01::Vector3(0, .77703, -3.24292),
                vb01::Vector3(0, 0.96741, -1.67253), 
                vb01::Vector3(0, 0.77703, 3.29185),
                vb01::Vector3(0.90322, 0.78121, -0.17934), 
                vb01::Vector3(0.90322, 0.78121, 0.42676), 
                vb01::Vector3(0.90322, 0.78121, 1.03286),
                vb01::Vector3(0.90322, 0.78121, 1.63896), 
                vb01::Vector3(0.90322, 0.78121, 2.24506), 
                vb01::Vector3(-0.90322, 0.78121, -0.17934),
                vb01::Vector3(-0.90322, 0.78121, 0.42676), 
                vb01::Vector3(-0.90322, 0.78121, 1.03286), 
                vb01::Vector3(-0.90322, 0.78121, 1.63896),
                vb01::Vector3(-0.90322, 0.78121, 2.24506)
            },
            {
								vb01::Vector3(0, 0.70974, -2.73371), 
                vb01::Vector3(0, 0.915258, -1.62768), 
                vb01::Vector3(0, 0.70974, 3.33251),
                vb01::Vector3(0.58824, 0.76908, 0.30145), 
                vb01::Vector3(0.58824, 0.98059, 0.58992), 
                vb01::Vector3(0.58824, 0.76908, 0.8634), 
                vb01::Vector3(0.58824, 0.98059, 1.13294), 
                vb01::Vector3(0.58824, 0.76908, 1.40591),
                vb01::Vector3(-0.58824, 0.76908, 0.30145), 
                vb01::Vector3(-0.58824, 0.98059, 0.58992), 
                vb01::Vector3(-0.58824, 0.76908, 0.8634), 
                vb01::Vector3(-0.58824, 0.98059, 1.13294), 
                vb01::Vector3(-0.58824, 0.76908, 1.40591)
            },
            {
								vb01::Vector3(0, 0.04967, -2.08348), 
                vb01::Vector3(0, 0.25291, -1.26886), 
                vb01::Vector3(0, 0.25291, 1.61533), 
                vb01::Vector3(0, 0.04967, 2.42996)
            },
            {
								vb01::Vector3(0, 0.53012, -2.92538), 
                vb01::Vector3(0, 0.707, -2.45061), 
                vb01::Vector3(0, 0.55879, 2.16916), 
                vb01::Vector3(0, 0.3808, 2.72071)
            },
            {
								vb01::Vector3(0, 0.73788, -2.79825), 
                vb01::Vector3(0, 0.73788, 2.79825)
            },
            {
								vb01::Vector3(0, 0.52632, -3.64706), 
                vb01::Vector3(0, 0.71625, -2.81714), 
                vb01::Vector3(0, 0.86667, -1.92772), 
                vb01::Vector3(0, 0.7724, 2.53618), 
                vb01::Vector3(0, 0.52632, 3.41345)
            }
        };
        const float turretAngleOffset[numberOfUnits][maxNumTurrets]{
            {0, 0, 180, -90, -90, -90, -90, -90, 90, 90, 90, 90, 90},
            {0, 0, 180, -90, -90, -90, -90, -90, 90, 90, 90, 90, 90},
            {0, 0, 180, 180},
            {0, 0, 180, 180},
            {0, 180},
            {0, 0, 0, 180, 180}
        };
        const float turretMaxAngle[numberOfUnits][maxNumTurrets]{
            {135, 135, 135, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45},
            {135, 135, 135, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45},
            {135, 135, 135, 135},
            {135, 135, 135, 135},
            {135, 135},
            {135, 135, 135, 135, 135}
        };
        const float turretRotationSpeed[numberOfUnits][maxNumTurrets]{
            {1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {2, 2, 2, 2},
            {2, 2, 2, 2},
            {2, 2},
            {2, 2, 2, 2, 2}
        };
        const int turretDamage[numberOfUnits][maxNumTurrets]{
            {200, 200, 200, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50},
            {200, 200, 200, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50},
            {100, 100, 100, 100},
            {100, 100, 100, 100},
            {90, 90},
            {90, 90, 90, 90, 90}
        };
        const int turretRateOfFire[numberOfUnits][maxNumTurrets]{
            {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
            {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
            {800, 800, 800, 800},
            {800, 800, 800, 800},
            {500,500},
            {700,700,700,700,700}
        };
        const float turretAxisLength[numberOfUnits][maxNumTurrets]{
            {5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {5, 5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {3, 3, 3, 3},
            {3, 3, 3, 3},
            {3, 3},
            {3, 3, 3, 3, 3},
        };
        const int numberOfMantlets[numberOfUnits][maxNumTurrets]{
            {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {2, 2, 2, 2},
            {2, 2, 2, 2},
            {1, 1},
            {3, 3, 3, 3, 3}
        };
        const std::string mantletNames[numberOfUnits][maxNumTurrets]{
            {
                "bigMantlet00", 
                "bigMantlet00", 
                "bigMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00", 
                "smallMantlet00"
            },
            {
                "bigMantlet01", 
                "bigMantlet01", 
                "bigMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01", 
                "smallMantlet01"
            },
            {"mantlet00", "mantlet00", "mantlet00", "mantlet00"},
            {"mantlet01", "mantlet01", "mantlet01", "mantlet01"},
            {"mantlet00", "mantlet00"},
            {"mantlet01", "mantlet01", "mantlet01", "mantlet01", "mantlet01"}
        };
        const vb01::Vector3 mantletPos[numberOfUnits][maxNumTurrets]{
            {
								vb01::Vector3(0, 0.19722f, -0.38441f),
                vb01::Vector3(0, 0.183f, -0.38876f), 
                vb01::Vector3(0, 0.183f, -0.38876f),
                vb01::Vector3(0, 0.06484f, -0.08587f), 
                vb01::Vector3(0, 0.06484f, -0.08587f), 
                vb01::Vector3(0, 0.06484f, -0.08587f),
                vb01::Vector3(0, 0.06484f, -0.08587f), 
                vb01::Vector3(0, 0.06484f, -0.08587f), 
                vb01::Vector3(0, 0.06484f, -0.08587f),
                vb01::Vector3(0, 0.06484f, -0.08587f), 
                vb01::Vector3(0, 0.06484f, -0.08587f), 
                vb01::Vector3(0, 0.06484f, -0.08587f),
                vb01::Vector3(0, 0.06484f, -0.08587f)
            },
            {
								vb01::Vector3(0, 0.111116, -0.335219), 
                vb01::Vector3(0, 0.111116, -0.335219), 
                vb01::Vector3(0, 0.111116, -0.335219),
                vb01::Vector3(0, 0.025217, -0.071749), 
                vb01::Vector3(0, 0.025217, -0.071749), 
                vb01::Vector3(0, 0.025217, -0.071749),
                vb01::Vector3(0, 0.025217, -0.071749), 
                vb01::Vector3(0, 0.025217, -0.071749), 
                vb01::Vector3(0, 0.025217, -0.071749),
                vb01::Vector3(0, 0.025217, -0.071749), 
                vb01::Vector3(0, 0.025217, -0.071749), 
                vb01::Vector3(0, 0.025217, -0.071749),
                vb01::Vector3(0, 0.025217, -0.071749)
            },
            {
								vb01::Vector3(0, 0.22782f, -0.12394f), 
                vb01::Vector3(0, 0.22782f, -0.12394f), 
                vb01::Vector3(0, 0.22782f, -0.12394f), 
                vb01::Vector3(0, 0.22782f, -0.12394f)
            },
            {
								vb01::Vector3(0, 0.14613, -0.0217), 
                vb01::Vector3(0, 0.14613, -0.0217), 
                vb01::Vector3(0, 0.14613, -0.0217), 
                vb01::Vector3(0, 0.14613, -0.0217)
            },
            {
								vb01::Vector3(0, 0.2842f, -0.1274f), 
                vb01::Vector3(0, 0.2842f, -0.1274f)
            },
            {
								vb01::Vector3(0, 0.09455, -0.19379), 
                vb01::Vector3(0, 0.09455, -0.19379), 
                vb01::Vector3(0, 0.09455, -0.19379), 
                vb01::Vector3(0, 0.09455, -0.19379), 
                vb01::Vector3(0, 0.09455, -0.19379)
            }
        };
        const float mantletPosSideOffset[numberOfUnits][maxNumTurrets]{
            {
                0.23346f, 
                0.23346f, 
                0.23346f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f, 
                0.04725f
            },
            {
                0.176833, 
                0.176833, 
                0.176833, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001, 
                0.030001
            },
            {
                0.07673f, 
                0.07673f, 
                0.07673f, 
                0.07673f
            },
            {
                0.08095, 
                0.08095, 
                0.08095, 
                0.08095
            },
            {0, 0},
            {0.11829, 0.11829, 0.11829, 0.11829, 0.11829}
        };
        const float mantletMaxAngle[numberOfUnits][maxNumTurrets]{
            {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45},
            {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45},
            {40, 40, 40, 40},
            {40, 40, 40, 40},
            {40, 40},
            {40, 40, 40, 40, 40}
        };
        const vb01::Vector3 barrelPos[numberOfUnits][maxNumTurrets]{
            {
								vb01::Vector3(0, 0, -0.16689f), 
                vb01::Vector3(0, 0, -0.16689f), 
                vb01::Vector3(0, 0, -0.16689f),
                vb01::Vector3(0, 0, -0.1021f),
                vb01::Vector3(0, 0, -0.1021f), 
                vb01::Vector3(0, 0, -0.1021f),
                vb01::Vector3(0, 0, -0.1021f), 
                vb01::Vector3(0, 0, -0.1021f), 
                vb01::Vector3(0, 0, -0.1021f),
                vb01::Vector3(0, 0, -0.1021f), 
                vb01::Vector3(0, 0, -0.1021f), 
                vb01::Vector3(0, 0, -0.1021f),
                vb01::Vector3(0, 0, -0.1021f)},
            {
								vb01::Vector3(0, 0, -0.246837), 
                vb01::Vector3(0, 0, -0.246837), 
                vb01::Vector3(0, 0, -0.246837),
                vb01::Vector3(0, 0, -0.054993), 
                vb01::Vector3(0, 0, -0.054993), 
                vb01::Vector3(0, 0, -0.054993),
                vb01::Vector3(0, 0, -0.054993), 
                vb01::Vector3(0, 0, -0.054993), 
                vb01::Vector3(0, 0, -0.054993),
                vb01::Vector3(0, 0, -0.054993), 
                vb01::Vector3(0, 0, -0.054993), 
                vb01::Vector3(0, 0, -0.054993),
                vb01::Vector3(0, 0, -0.054993)
            },
            {
								vb01::Vector3(0, 0, -0.12806f), 
                vb01::Vector3(0, 0, -0.12806f), 
                vb01::Vector3(0, 0, -0.12806f), 
                vb01::Vector3(0, 0, -0.12806f)},
            {
								vb01::Vector3(0, 0, -0.11048), 
                vb01::Vector3(0, 0, -0.11048), 
                vb01::Vector3(0, 0, -0.11048),
                vb01::Vector3(0, 0, -0.11048)
            },
            {
								vb01::Vector3(0, 0, -0.23015f), 
                vb01::Vector3(0, 0, -0.23015f)
            },
            {
								vb01::Vector3(0, 0, -0.14595), 
                vb01::Vector3(0, 0, -0.14595), 
                vb01::Vector3(0, 0, -0.14595), 
                vb01::Vector3(0, 0, -0.14595), 
                vb01::Vector3(0, 0, -0.14595)
            }
        };
        const std::string barrelNames[numberOfUnits][maxNumTurrets]{
            {
                "bigBarrel00", 
                "bigBarrel00", 
                "bigBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00", 
                "smallBarrel00"
            },
            {
                "bigBarrel01", 
                "bigBarrel01", 
                "bigBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01", 
                "smallBarrel01"
            },
            {"barrel00", "barrel00", "barrel00", "barrel00"},
            {"barrel01", "barrel01", "barrel01", "barrel01"},
            {"barrel00", "barrel00"},
            {"barrel01", "barrel01", "barrel01", "barrel01", "barrel01"}
        };
        const vb01::Vector3 guidedMissilePos[numberOfUnits][6]{
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
