#include "util.h"
#include "gameManager.h"
#include "stateManager.h"
#include "guiAppState.h"

using namespace game::core;

int main() {
    GameManager *gameManager = GameManager::getSingleton();
		/*
    GuiAppState *state = new GuiAppState();
    gameManager->getStateManager()->attachState(state);
    makeTitlescreenButtons(state);
		*/
    return 0;
}
