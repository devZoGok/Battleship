#include <algorithm>
#include <root.h>

#include "gameManager.h"
#include "guiAppState.h"
#include "stateManager.h"
#include "eventListener.h"

using namespace std;
using namespace game::gui;
using namespace vb01;

namespace game{
    namespace core{
				static GameManager *gameManager = nullptr;

				GameManager* GameManager::getSingleton(){
						if(!gameManager)
								gameManager = new GameManager();

						return gameManager;
				}

        GameManager::GameManager() {
        }

        GameManager::~GameManager() {}

				void GameManager::start() {
						Root *root = Root::getSingleton();
						root->start(800, 600, "../../vb01", "Battleship");

						stateManager = new StateManager();
            listener = new InputManager(root->getWindow());
				}

        void GameManager::update() {
						Root::getSingleton()->update();
            listener->update();
						stateManager->update();

						/*
            for (BitmapText *b : bitmapTexts)
                b->update();

            for (Image *i : images)
                i->update();
								*/
        }
    }
}
