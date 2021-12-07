#include "util.h"
#include "gameManager.h"
#include "guiAppState.h"

#include <stateManager.h>

using namespace battleship;

int main() {
    GameManager *gameManager = GameManager::getSingleton();
		gameManager->start(800, 600);
    GuiAppState *state = new GuiAppState();
    gameManager->getStateManager()->attachAppState(state);
    makeTitlescreenButtons(state);

		while(true){
				gameManager->update();
		}
    return 0;
}
