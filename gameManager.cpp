#include <algorithm>
#include <root.h>

#include "gameManager.h"
#include "guiAppState.h"
#include "stateManager.h"
#include "inputManager.h"

using namespace std;
using namespace vb01;

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
				this->width = 800;
				this->height = 600;

				Root *root = Root::getSingleton();
				root->start(width, height, "../../vb01/", "Battleship");

				stateManager = new StateManager();
        listener = new InputManager(root->getWindow());
		}

    void GameManager::update() {
				Root::getSingleton()->update();
        listener->update();
				stateManager->update();
    }
}
