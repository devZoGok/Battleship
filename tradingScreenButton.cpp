#include "tradingScreenButton.h"
#include "concreteGuiManager.h"
#include "offerButton.h"
#include "gameManager.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;

	TradingScreenButton::TradingScreenButton(Vector2 pos, Vector2 size, int plId, string gs, string name, int trigger, string imagePath) : ActiveStateButton(pos, size, gs, name, trigger, imagePath), playerId(plId){}

	//TODO add method to get buttons by name
	void TradingScreenButton::onClick(){
		buttons.push_back(this);
		ActiveStateButton::onClick();

		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		int numButtons = guiManager->getButtons().size();
		((OfferButton*)guiManager->getButtons()[numButtons - 1])->setPlayerId(playerId);
		guiManager->removeButton(this);
	}
}
