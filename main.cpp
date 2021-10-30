
#include "util.h"
#include "gameManager.h"
#include "stateManager.h"
#include "guiAppState.h"

using namespace irr;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace game::core;
using namespace game::util;
using namespace irr::video;

int main() {
    GameManager *gameManager = GameManager::getSingleton();
    GuiAppState *state = new GuiAppState();
    gameManager->getStateManager()->attachState(state);
    makeTitlescreenButtons(state);
		IrrlichtDevice *device = gameManager->getDevice();
		IVideoDriver *driver = device->getVideoDriver();

    while (device->run()) {
        driver->beginScene(true, true, 0);
        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();
        gameManager->update();
        driver->endScene();
    }

    device->drop();
    return 0;
}
