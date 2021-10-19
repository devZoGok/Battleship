#pragma once
#ifndef DEF_CONFIGS_H
#define DEF_CONFIGS_H

#include<irrlicht.h>
#include<string>
#include"Key.h"

namespace game{
    namespace core{
        const irr::io::path PATH = "/home/dominykas/c++/Game/Assets/";
        const std::string PATH_STR = "/home/dominykas/c++/Game/Assets/";
        const double camPanSpeed = .1;
        const irr::io::path DEFAULT_TEXTURE = PATH + "Textures/defaultTexture.jpg";

        const static int numAppStates=3,numMaxBinds=23;
        static const int numBinds[numAppStates]{12,1,23};
        static const int numConfBinds[numAppStates]{0,1,13};
        const static Bind binds[numAppStates][numMaxBinds]{
            {
                LEFT_CLICK,
                SCROLLING_UP,
                SCROLLING_DOWN,
                LEFT,
                RIGHT,
                DELETE_CHAR,
                CAPS_LOCK,
                SHIFT_CAPS,
                SPACE,
                PLUS,
                MINUS,
                DEVSTERISK
            },
            {
                TOGGLE_MAIN_MENU
            },
            {
                HALT,
                ZOOM_IN,
                ZOOM_OUT,
                LOOK_AROUND,
                DRAG_BOX,
                DESELECT,
                LEFT_CONTROL,
                LEFT_SHIFT,
                SELECT_PATROL_POINTS,
                LAUNCH,
                TOGGLE_SUB,
                INSTALL_AAM,
                INSTALL_AWM,
                GROUP_0,
                GROUP_1,
                GROUP_2,
                GROUP_3,
                GROUP_4,
                GROUP_5,
                GROUP_6,
                GROUP_7,
                GROUP_8,
                GROUP_9
            }
        };
        const static int triggers[numAppStates][numMaxBinds]{
            {
                0,
                3,
                4,
                irr::KEY_LEFT,
                irr::KEY_RIGHT,
                irr::KEY_BACK,
                irr::KEY_CAPITAL,
                irr::KEY_LSHIFT,
                irr::KEY_SPACE,
                irr::KEY_PLUS,
                irr::KEY_MINUS,
                irr::KEY_OEM_3
            },
            {
                irr::KEY_ESCAPE
                
            },
            {
                irr::KEY_KEY_H, 
                3, 
                4, 
                1, 
                0, 
                2, 
                irr::KEY_LCONTROL, 
                irr::KEY_LSHIFT, 
                irr::KEY_KEY_P, 
                irr::KEY_KEY_C, 
                irr::KEY_KEY_S,
                irr::KEY_KEY_A,
                irr::KEY_KEY_W,
                irr::KEY_KEY_0,
                irr::KEY_KEY_1,
                irr::KEY_KEY_2,
                irr::KEY_KEY_3,
                irr::KEY_KEY_4,
                irr::KEY_KEY_5,
                irr::KEY_KEY_6,
                irr::KEY_KEY_7,
                irr::KEY_KEY_8,
                irr::KEY_KEY_9
            }
        };
        const static bool isKey[numAppStates][numMaxBinds]{
            {0,0,0,1,1,1,1,1,1,1,1,1},
            {1},
            {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1}
        };
        const static bool isAnalog[numAppStates][numMaxBinds]{
            {0,0,0,0,0,0,0,0,0,0,0,0},
            {0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0}
        };
//         const int guiBindsNumber = 3;
//         const std::string guiAppStateBinds[guiBindsNumber]{"leftClick", "upScroll", "downScroll"};
//         const int guiAppStateTriggers[guiBindsNumber]{0, 1, 1};
//         const bool guiAppStateIsKey[guiBindsNumber]{0, 0, 0};
//         const bool guiAppStateIsAnalog[guiBindsNumber]{0, 0, 0};
//         const std::string guiAppStateKeys[guiBindsNumber]{"LeftMouse", "UpScroll", "DownScroll"};
// 
//         const int inGameAppStateBindsNumber = 1;
//         const std::string inGameAppStateBinds[inGameAppStateBindsNumber]{"toggleMainMenu"};
//         const int inGameAppStateTriggers[inGameAppStateBindsNumber]{irr::KEY_ESCAPE};
//         const bool inGameAppStateIsKey[inGameAppStateBindsNumber]{1};
//         const bool inGameAppStateIsAnalog[inGameAppStateBindsNumber]{0};
//         const std::string inGameAppStateKeys[inGameAppStateBindsNumber]{"Esc"};
// 
//         const int activeGameStateBindsNumber = 10;
//         const std::string activeGameStateBinds[activeGameStateBindsNumber]{
//             "halt", 
//             "zoomIn", 
//             "zoomOut", 
//             "lookAround", 
//             "dragBox", 
//             "deselect", 
//             "leftControl", 
//             "selectingPatrolPoints", 
//             "launch", 
//             "toggleSub"
//         };
//         const int activeGameStateTriggers[activeGameStateBindsNumber]{
//             irr::KEY_KEY_H, 
//             3, 
//             4, 
//             1, 
//             0, 
//             2, 
//             irr::KEY_LCONTROL, 
//             irr::KEY_KEY_P, 
//             irr::KEY_KEY_C, 
//             irr::KEY_KEY_S
//         };
//         const bool activeGameStateIsKey[activeGameStateBindsNumber]{1, 0, 0, 0, 0, 0, 1, 1, 1, 1};
//         const bool activeGameStateIsAnalog[activeGameStateBindsNumber]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//         const std::string activeGameStateKeys[activeGameStateBindsNumber]{
//             "H", 
//             "UpScroll", 
//             "DownScroll", 
//             "MiddleMouse", 
//             "LeftMouse", 
//             "RightMouse", 
//             "leftControl", 
//             "P",
//             "C", 
//             "S"
//         };
    }
}

#endif
