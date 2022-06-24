#include <algorithm>

#include <root.h>

#include <stateManager.h>
#include <inputManager.h>

#include <assetManager.h>

#include "gameManager.h"
#include "guiAppState.h"

using namespace std;
using namespace vb01;
using namespace gameBase;

namespace battleship{
		static GameManager *gameManager = nullptr;

		GameManager* GameManager::getSingleton(){
				if(!gameManager)
						gameManager = new GameManager();

				return gameManager;
		}

    GameManager::GameManager() {
    }

    GameManager::~GameManager() {}

		void GameManager::start(int width, int height) {
				this->width = width;
				this->height = height;

				Root *root = Root::getSingleton();
				root->start(width, height, "../../vb01/", "Battleship");

				stateManager = new StateManager();
        inputManager = new InputManager(stateManager, root->getWindow());
		}

    void GameManager::update() {
				Root::getSingleton()->update();
        inputManager->update();
				stateManager->update();
    }
}
