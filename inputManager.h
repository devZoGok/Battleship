#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include "gameManager.h"

class GLFWwindow;

namespace battleship{
		class StateManager;

    class InputManager {
    public:
        InputManager(GLFWwindow*);
        ~InputManager();
        //virtual bool OnEvent(const irr::SEvent&);
        void update();
    private:
        //irr::SEvent* event;
				GLFWwindow *window = nullptr;
				StateManager *stateManager = nullptr;
        bool keyEvent = false, mouseEvent = false;
    };
}

#endif
