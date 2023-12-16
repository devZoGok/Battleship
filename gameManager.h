#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "util.h"

#include <mapping.h>

#include <vector>
#include <string>

namespace gameBase{
    class InputManager;
    class StateManager;
}

namespace battleship{
    class GameManager {
    public:
		static GameManager* getSingleton();
		void start(std::string);
        void update();
        inline int getWidth(){return width;}
        inline int getHeight(){return height;}
		inline std::string getPath(){return path;}
        inline gameBase::InputManager* getInputManager(){return inputManager;}
        inline bool isServerSide(){return serverSide;}
		inline gameBase::StateManager* getStateManager(){return stateManager;}
    private:
        GameManager(){}
        ~GameManager(){}
		void registerOrderMembers();
		void registerVectorMembers();
		void registerQuaternionMembers();
		void registerGameObjectFactoryMembers();
		void registerUnitMembers();
		void registerPlayerMembers();
		void initLua(std::string);

		gameBase::StateManager *stateManager = nullptr;
		gameBase::InputManager *inputManager = nullptr;
        int width, height;
		std::string path = "";
        bool serverSide;
    };
    
}

#endif
