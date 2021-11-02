#include <cmath>

#include "eventListener.h"
#include "stateManager.h"

using namespace std;

namespace battleship{
	double *posX,*posY,strX,strY;
	int width,height;
	
	void foo(GLFWwindow *window,double newPosX,double newPosY){
		strX=(*posX-newPosX)/width,strY=(newPosY-*posY)/height;
	}

	InputManager::InputManager(GLFWwindow *window){
		this->stateManager = GameManager::getSingleton()->getStateManager();
		this->window = window;
		posX = new double, posY = new double;
	}

	InputManager::~InputManager(){}

	void InputManager::update(){
		GameManager *gm = GameManager::getSingleton();
		width = gm->getWidth(), height = gm->getHeight();

		glfwGetCursorPos(window,posX,posY);
		int joystick;
		int numAxis=3,numButtons=6;
		const u8 *buttons;
		const float *axis;

		if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
			joystick = GLFW_JOYSTICK_1;
			axis = glfwGetJoystickAxes(joystick, &numAxis);
			buttons = glfwGetJoystickButtons(joystick, &numButtons);
		}

		for(int j = 0; j < stateManager->getAppStates().size(); j++){
			AbstractAppState *a = stateManager->getAppStates()[j];

			for(int i = 0; i < a->getKeysNumber(); i++){
				Mapping *m = a->getKey(i);

				if(m->action){
					bool pressed;

					switch(m->type){
						case Mapping::KEYBOARD:
							pressed=glfwGetKey(window,m->trigger);
							break;
						case Mapping::MOUSE_KEY:
							pressed=glfwGetMouseButton(window,m->trigger);
							break;
						case Mapping::JOYSTICK_KEY:
							if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
								pressed=buttons[m->trigger];
							}
							break;
					}

					if((pressed&&!m->pressed)||(!pressed&&m->pressed)){
						m->pressed=pressed;	
						a->onAction(m->bind,pressed);
					}
				}
				else{
					switch(m->type){
						case Mapping::MOUSE_AXIS:
							{
								float str;
								switch(m->trigger){
									case Mapping::MOUSE_AXIS_LEFT:
									case Mapping::MOUSE_AXIS_RIGHT:
										str=strX;
										break;
									case Mapping::MOUSE_AXIS_UP:
									case Mapping::MOUSE_AXIS_DOWN:
										str=strY;
										break;
								}
								a->onAnalog(m->bind,str);
								break;
							}
						case Mapping::JOYSTICK_AXIS:
							{
								int axisId = (m->trigger - (m->trigger % 2 > 0)) / 2;
								float str = fabs(axis[axisId]) >= .1 ? axis[axisId] : 0;

								if(fabs(str)>0)
									a->onAnalog(m->bind,str);

								break;
							}
					}
				}
			}

			for(int i = 0; i < 350; i++){
				if(glfwGetKey(window, i))
					a->onRawKeyPress(i);
				else if(glfwGetMouseButton(window, i))
					a->onRawMousePress(i);
			}

			glfwSetCursorPosCallback(window,foo);

			if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
				for(int i = 0; i < numAxis; i++)
					if(abs(axis[i]) == 1)
						a->onRawJoystickAxis(i, axis[i]);

				for(int i = 0; i < numButtons; i++)
					if(buttons[i])
						a->onRawJoystickKeyPress(i);
			}
		}
    }
}
