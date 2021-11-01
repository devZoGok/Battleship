#include "util.h"
#include "gameManager.h"
#include "stateManager.h"
#include "guiAppState.h"

using namespace battleship;

int main() {
    GameManager *gameManager = GameManager::getSingleton();
		gameManager->start(800, 600);
    GuiAppState *state = new GuiAppState();
    gameManager->getStateManager()->attachState(state);
    makeTitlescreenButtons(state);

		while(true){
				gameManager->update();
		}
    return 0;
}
