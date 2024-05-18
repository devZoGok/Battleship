#include "offerButton.h"
#include "concreteGuiManager.h"
#include "gameManager.h"
#include "activeGameState.h"
#include "player.h"

#include <stateManager.h>

#include <textbox.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	OfferButton::OfferButton(Vector3 pos, Vector2 size, int plId, string name, int trigger, string imagePath) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, true, imagePath), playerId(plId){}

	void OfferButton::onClick(){
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		vector<Textbox*> textboxes = guiManager->getTextboxes();
	}
}
