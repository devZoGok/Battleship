#include "statsButton.h"
#include "gameManager.h"
#include "concreteGuiManager.h"
#include "inGameAppState.h"
#include "game.h"
#include "map.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	StatsButton::StatsButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, true, imagePath){}

	void StatsButton::addPlayerDataGuiElements(int playerId){
		string fontPath = GameManager::getSingleton()->getPath() + "Fonts/batang.ttf";
		Player *player = Game::getSingleton()->getPlayer(playerId);

		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() + "text");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));

		const int numPairs = 7;
		pair<string, wstring> unitDataPairs[numPairs]{
			make_pair("pl", stringToWstring(player->getName())),
			make_pair("vb", to_wstring(player->getNumVehiclesBuilt())),
			make_pair("vd", to_wstring(player->getNumVehiclesDestroyed())),
			make_pair("vl", to_wstring(player->getNumVehiclesLost())),
			make_pair("sb", to_wstring(player->getNumStructuresBuilt())),
			make_pair("sd", to_wstring(player->getNumStructuresDestroyed())),
			make_pair("sl", to_wstring(player->getNumStructuresLost())),
		};
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		Vector3 plCol = player->getColor();

		for(int i = 0; i < numPairs; i++){
			Text *category = guiManager->getText(unitDataPairs[i].first);

			Material *categoryMat = new Material(root->getLibPath() + "text");
			categoryMat->addBoolUniform("texturingEnabled", false);
			categoryMat->addVec4Uniform("diffuseColor", Vector4(plCol.x, plCol.y, plCol.z, 1));

			Text *categoryVal = new Text(fontPath, unitDataPairs[i].second);
			categoryVal->setScale(category->getScale());
			categoryVal->setMaterial(categoryMat);

			Node *categoryValNode = new Node(category->getNode()->getPosition() + Vector3(0, 100 * playerId, 0));
			categoryValNode->addText(categoryVal);
			root->getGuiNode()->attachChild(categoryValNode);

			guiManager->addText(categoryVal);
		}
	}

	void StatsButton::onClick(){
		ConcreteGuiManager::getSingleton()->readLuaScreenScript("statistics.lua");

		Map::getSingleton()->unload();

		StateManager *sm = GameManager::getSingleton()->getStateManager();
	   	InGameAppState *inGameState = (InGameAppState*)sm->getAppStateByType(int(AppStateType::IN_GAME_STATE));
		sm->dettachAppState(inGameState);
		delete inGameState;

		Game *game = Game::getSingleton();
		int numPlayers = game->getNumPlayers();

		for(int i = 1; i < numPlayers; i++)
			addPlayerDataGuiElements(i);

		game->removeAllElements();
	}
}
