#include "util.h"
#include "gameManager.h"
#include "guiAppState.h"

#include <stateManager.h>

using namespace battleship;
using namespace vb01;

int main() {
    GameManager *gameManager = GameManager::getSingleton();
		gameManager->start(1920, 1080);
    GuiAppState *state = new GuiAppState();
    gameManager->getStateManager()->attachAppState(state);

    makeTitlescreenButtons(state);

		while(true){
				gameManager->update();
		}
    return 0;
}
