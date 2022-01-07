#ifndef DEF_CONFIGS_H
#define DEF_CONFIGS_H

#include <string>
#include <glfw3.h>

#include <mapping.h>

#include "binds.h"

namespace battleship{
		namespace configData{
				using namespace gameBase;

    const std::string PATH = "/home/dominykas/c++/Battleship/Assets/";
    const std::string DEFAULT_TEXTURE = PATH + "Textures/defaultTexture.jpg";
    const double camPanSpeed = .1;

    const static int numAppStates = 3, numMaxBinds = 23;
    static const int numBinds[numAppStates]{12, 1, 23};
    static const int numConfBinds[numAppStates]{0, 1, 13};

    const static Bind binds[numAppStates][numMaxBinds]{
        {
						Bind::LEFT_CLICK,
            Bind::SCROLLING_UP,
            Bind::SCROLLING_DOWN,
            Bind::LEFT,
            Bind::RIGHT,
            Bind::DELETE_CHAR,
        },
        {
						Bind::TOGGLE_MAIN_MENU
        },
        {
            Bind::LOOK_AROUND,
            Bind::DRAG_BOX,
            Bind::DESELECT,
						Bind::HALT,
            Bind::ZOOM_IN,
            Bind::ZOOM_OUT,
            Bind::LEFT_CONTROL,
            Bind::LEFT_SHIFT,
            Bind::SELECT_PATROL_POINTS,
            Bind::LAUNCH,
            Bind::TOGGLE_SUB,
            Bind::INSTALL_AAM,
            Bind::INSTALL_AWM,
            Bind::GROUP_0,
            Bind::GROUP_1,
            Bind::GROUP_2,
            Bind::GROUP_3,
            Bind::GROUP_4,
            Bind::GROUP_5,
            Bind::GROUP_6,
            Bind::GROUP_7,
            Bind::GROUP_8,
            Bind::GROUP_9
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
            1, 
            0, 
            1, 
            GLFW_KEY_H, 
            3, 
            4, 
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
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1}
    };
    const static bool isAnalog[numAppStates][numMaxBinds]{
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0}
    };
		}
}

#endif
