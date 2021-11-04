#ifndef DEF_CONFIGS_H
#define DEF_CONFIGS_H

#include <string>
#include <glfw3.h>

#include "key.h"

namespace battleship{
		namespace configData{
    const std::string PATH = "/home/dominykas/c++/Battleship/Assets/";
    const std::string DEFAULT_TEXTURE = PATH + "Textures/defaultTexture.jpg";
    const double camPanSpeed = .1;

    const static int numAppStates = 3, numMaxBinds = 23;
    static const int numBinds[numAppStates]{12, 1, 23};
    static const int numConfBinds[numAppStates]{0, 1, 13};
    const static Mapping::Bind binds[numAppStates][numMaxBinds]{
        {
						Mapping::LEFT_CLICK,
            Mapping::SCROLLING_UP,
            Mapping::SCROLLING_DOWN,
            Mapping::LEFT,
            Mapping::RIGHT,
            Mapping::DELETE_CHAR,
            Mapping::CAPS_LOCK,
            Mapping::SHIFT_CAPS,
            Mapping::SPACE,
            Mapping::PLUS,
            Mapping::MINUS,
            Mapping::DEVSTERISK
        },
        {
						Mapping::TOGGLE_MAIN_MENU
        },
        {
						Mapping::HALT,
            Mapping::ZOOM_IN,
            Mapping::ZOOM_OUT,
            Mapping::LOOK_AROUND,
            Mapping::DRAG_BOX,
            Mapping::DESELECT,
            Mapping::LEFT_CONTROL,
            Mapping::LEFT_SHIFT,
            Mapping::SELECT_PATROL_POINTS,
            Mapping::LAUNCH,
            Mapping::TOGGLE_SUB,
            Mapping::INSTALL_AAM,
            Mapping::INSTALL_AWM,
            Mapping::GROUP_0,
            Mapping::GROUP_1,
            Mapping::GROUP_2,
            Mapping::GROUP_3,
            Mapping::GROUP_4,
            Mapping::GROUP_5,
            Mapping::GROUP_6,
            Mapping::GROUP_7,
            Mapping::GROUP_8,
            Mapping::GROUP_9
        }
    };
    const static int triggers[numAppStates][numMaxBinds]{
        {
            0,
            3,
            4,
            GLFW_KEY_LEFT,
            GLFW_KEY_RIGHT,
            GLFW_KEY_BACKSPACE,
            GLFW_KEY_CAPS_LOCK,
            GLFW_KEY_LEFT_SHIFT,
            GLFW_KEY_SPACE,
            GLFW_KEY_KP_ADD,
            GLFW_KEY_MINUS,
            GLFW_KEY_3
        },
        {
            GLFW_KEY_ESCAPE
            
        },
        {
            GLFW_KEY_H, 
            3, 
            4, 
            1, 
            0, 
            2, 
            GLFW_KEY_LEFT_CONTROL, 
            GLFW_KEY_LEFT_SHIFT, 
            GLFW_KEY_P, 
            GLFW_KEY_C, 
            GLFW_KEY_S,
            GLFW_KEY_A,
            GLFW_KEY_W,
            GLFW_KEY_0,
            GLFW_KEY_1,
            GLFW_KEY_2,
            GLFW_KEY_3,
            GLFW_KEY_4,
            GLFW_KEY_5,
            GLFW_KEY_6,
            GLFW_KEY_7,
            GLFW_KEY_8,
            GLFW_KEY_9
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
		}
}

#endif
