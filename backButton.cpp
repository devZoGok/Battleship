#include "backButton.h"
#include "gameManager.h"
#include "concreteGuiManager.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;

    BackButton::BackButton(Vector2 pos, Vector2 size, string name, string screen) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", -1, true) {
		this->screen = screen;
	}

    void BackButton::onClick() {
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		guiManager->removeAllButtons(vector<Button*>{this});
		guiManager->removeAllListboxes();
		guiManager->removeAllCheckboxes();
		guiManager->removeAllSliders();
		guiManager->removeAllTextboxes();

		guiManager->readLuaScreenScript(screen);

		guiManager->removeButton(this);
	}
}
