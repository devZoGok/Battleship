#include "playerTradeButton.h"
#include "activeGameState.h"
#include "concreteGuiManager.h"
#include "game.h"

#include <stateManager.h>
#include <solUtil.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	PlayerTradeButton::PlayerTradeButton(Vector2 pos, Vector2 size, string gs, string name, int trigger, string imagePath) : ActiveStateButton(pos, size, gs, name, trigger, imagePath){}

	void PlayerTradeButton::onClick(){
		ActiveStateButton::onClick();

		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		ActiveGameState *activeState = (ActiveGameState*)stateManager->getAppStateByType((int)AppStateType::ACTIVE_STATE);
		Player *mainPlayer = activeState->getPlayer();

		vector<Player*> players = Game::getSingleton()->getPlayers();
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		sol::state_view SOL_LUA_VIEW = generateView();

		for(int i = 0, plId = 0; i < players.size(); i++){
			if(players[i] != mainPlayer){
				vector<Button*> buttons = guiManager->getButtons(); 
				vector<Listbox*> listboxes = guiManager->getListboxes(); 
				vector<Checkbox*> checkboxes = guiManager->getCheckboxes(); 
				vector<Slider*> sliders = guiManager->getSliders(); 
				vector<Textbox*> textboxes = guiManager->getTextboxes(); 
				vector<Node*> guiRects = guiManager->getGuiRectangles(); 
				vector<Text*> texts = guiManager->getTexts(); 

				SOL_LUA_VIEW.script("lineId = " + to_string(plId));
				guiManager->readLuaScreenScript("tradingPlayerGuiTray.lua", buttons, listboxes, checkboxes, sliders, textboxes, guiRects, texts);

				int numButtons = guiManager->getButtons().size();
				int numListboxes = guiManager->getListboxes().size();
				int numTexts = guiManager->getTexts().size();
				guiManager->getTexts()[numTexts - 1]->setText(stringToWstring(players[i]->getName()));

				plId++;
			}
		}
	}
}
