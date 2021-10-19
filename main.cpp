#include<irrlicht.h>
#include"util.h"
#include"GameManager.h"
#include"GuiAppState.h"

using namespace irr;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace game::core;
using namespace game::util;
using namespace irr::video;

int main() {
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32, false, true, true, nullptr);
    device->setResizable(true);
    IVideoDriver *driver = device->getVideoDriver();
    ISceneManager *smgr = device->getSceneManager();
    IGUIEnvironment *guiEnv = device->getGUIEnvironment();
    device->setWindowCaption(L"(((A)))");
    GameManager *gameManager = new GameManager(device);
    GuiAppState *state = new GuiAppState(gameManager);
    gameManager->attachState(state);
    makeTitlescreenButtons(gameManager, state);
    //smgr->getMesh(PATH+"Models/Battel");
    while (device->run()) {
        driver->beginScene(true, true, 0);
        smgr->drawAll();
        guiEnv->drawAll();
        gameManager->update();
        driver->endScene();
    }
    device->drop();
    delete gameManager;
    return 0;
}
