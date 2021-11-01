#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "defConfigs.h"
#include "abstractAppState.h"
#include "key.h"
#include "util.h"
#include <vector>
#include <string>

namespace battleship{
    class InputManager;
    class StateManager;
    
    class GameManager {
    public:
				static GameManager* getSingleton();
				void start(int, int);
        void update();
        inline int getWidth(){return width;}
        inline int getHeight(){return height;}
        inline InputManager* getListener(){return listener;}
        inline bool isServerSide(){return serverSide;}
				inline StateManager* getStateManager(){return stateManager;}
    private:
        GameManager();
        ~GameManager();

				StateManager *stateManager = nullptr;
        std::vector<AbstractAppState*> appStates;
        int width, height;
        InputManager *listener = nullptr;
        bool serverSide;
    };
    
}

#endif
