#ifndef DEF_CONFIGS_H
#define DEF_CONFIGS_H

#include <string>
#include <vector>

#include <glfw3.h>

#include <mapping.h>

#include "binds.h"
#include "gameManager.h"

namespace battleship{
	namespace configData{
		using namespace gameBase;

  		const std::string DEFAULT_TEXTURE = GameManager::getSingleton()->getPath() + "Textures/defaultTexture.jpg";
  		const double camPanSpeed = .1;
		const int maxNumGroups = 10;

  		const static int numAppStates = 4;
  		const static int numStaticBinds[numAppStates]{6, 0, 5, 5};
  		const static int numConfBinds[numAppStates]{0, 1, 22, 0};
		const static int maxStaticBinds = 6;
		const static int maxConfBinds = 22;
		const static int numScripts = 5;

		const static std::string scriptPathBase = GameManager::getSingleton()->getPath() + "Scripts/";
		const static std::vector<std::string> scripts = std::vector<std::string>{
			scriptPathBase + "options.lua",
			scriptPathBase + "unitData.lua",
			scriptPathBase + "vehicleData.lua",
			scriptPathBase + "structureData.lua",
			scriptPathBase + "player.lua"
		};

  		const static Bind staticBinds[numAppStates][maxStaticBinds]{
  		    {
				Bind::LEFT_CLICK,
  		        Bind::SCROLLING_UP,
  		        Bind::SCROLLING_DOWN,
  		        Bind::LEFT,
  		        Bind::RIGHT,
  		        Bind::DELETE_CHAR,
  		    },
  		    {},
  		    {
  		        Bind::LOOK_UP,
  		        Bind::LOOK_DOWN,
  		        Bind::LOOK_LEFT,
  		        Bind::LOOK_RIGHT,
  		        Bind::LOOK_AROUND,
  		    },
			{
  		        Bind::LOOK_UP,
  		        Bind::LOOK_DOWN,
  		        Bind::LOOK_LEFT,
  		        Bind::LOOK_RIGHT,
  		        Bind::LOOK_AROUND,
			}
  		};
  		const static Bind confBinds[numAppStates][maxConfBinds]{
  		    {},
  		    {
				Bind::TOGGLE_MAIN_MENU
  		    },
  		    {
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
  		        Bind::GROUP_0,
  		        Bind::GROUP_1,
  		        Bind::GROUP_2,
  		        Bind::GROUP_3,
  		        Bind::GROUP_4,
  		        Bind::GROUP_5,
  		        Bind::GROUP_6,
  		        Bind::GROUP_7,
  		        Bind::GROUP_8,
  		        Bind::GROUP_9,
  		        Bind::SELECT_STRUCTURE,
  		        Bind::DESELECT_STRUCTURE
  		    },
			{}
  		};

  		const static int staticTriggers[numAppStates][maxStaticBinds]{
  		    {
  		        0,
				GLFW_KEY_W,
				GLFW_KEY_S,
  		        GLFW_KEY_LEFT,
  		        GLFW_KEY_RIGHT,
  		        GLFW_KEY_BACKSPACE
  		    },
  		    {},
  		    {
				Mapping::MOUSE_AXIS_UP,
				Mapping::MOUSE_AXIS_DOWN,
				Mapping::MOUSE_AXIS_LEFT,
				Mapping::MOUSE_AXIS_RIGHT,
				0
  		    },
			{
				Mapping::MOUSE_AXIS_UP,
				Mapping::MOUSE_AXIS_DOWN,
				Mapping::MOUSE_AXIS_LEFT,
				Mapping::MOUSE_AXIS_RIGHT,
				0
			}
  		};
  		const static int confTriggers[numAppStates][maxConfBinds]{
  		    {},
  		    {
  		        GLFW_KEY_ESCAPE
  		    },
  		    {
				GLFW_KEY_LEFT,
  		        GLFW_KEY_RIGHT,
  		        GLFW_KEY_H, 
  		        3, 
  		        4, 
  		        GLFW_KEY_LEFT_CONTROL, 
  		        GLFW_KEY_LEFT_SHIFT, 
  		        GLFW_KEY_P, 
  		        GLFW_KEY_C, 
  		        GLFW_KEY_S,
  		        GLFW_KEY_0,
  		        GLFW_KEY_1,
  		        GLFW_KEY_2,
  		        GLFW_KEY_3,
  		        GLFW_KEY_4,
  		        GLFW_KEY_5,
  		        GLFW_KEY_6,
  		        GLFW_KEY_7,
  		        GLFW_KEY_8,
  		        GLFW_KEY_9,
  		        GLFW_KEY_B,
  		        GLFW_KEY_ESCAPE
  		    },
			{}
  		};

  		const static bool isStaticKey[numAppStates][maxStaticBinds]{
  		    {0, 1, 1, 1, 1, 1},
  		    {},
			{0, 0, 0, 0, 1},
			{0, 0, 0, 0, 1}
  		};
  		const static bool isConfKey[numAppStates][maxConfBinds]{
			{},
  		    {1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{}
  		};

  		const static bool isStaticAction[numAppStates][maxStaticBinds]{
  		    {1, 1, 1, 1, 1, 1},
  		    {},
			{0, 0, 0, 0, 1},
			{0, 0, 0, 0, 1}
  		};
  		const static bool isConfAction[numAppStates][maxConfBinds]{
			{},
  		    {1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{}
  		};

		int calcSumStaticBinds(int, bool);
		int calcSumConfBinds(int, bool);
		int calcSumBinds(int, bool);
	}
}

#endif
