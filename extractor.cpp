#include "extractor.h"
#include "player.h"
#include "game.h"
#include "activeGameState.h"
#include "resourceDeposit.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	Extractor::Extractor(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus, ResourceDeposit *rd) : Structure(player, id, pos, rot, buildStatus){
		if(!rd){
			vector<ResourceDeposit*> deposits;

			for(Player *pl : Game::getSingleton()->getPlayers()){
				vector<ResourceDeposit*> dep = pl->getResourceDeposits();
				deposits.insert(deposits.end(), dep.begin(), dep.end());
			}

			for(ResourceDeposit *dep : deposits)
				if(dep->getPos().getDistanceFrom(pos) < .001){
					deposit = dep;
					break;
				}
		}

		Vector2 size = Vector2(lenHpBar, 10);
		ammountBackground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 0, 1));
		ammountForeground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 1, 1));
	}

	Extractor::~Extractor(){
		removeBar(ammountForeground);
		removeBar(ammountBackground);
	}

	void Extractor::update(){
		Structure::update();

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		vector<Player*> selectingPlayers = getSelectingPlayers();
		bool mainPlayerSelecting = (activeState && find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());
		int ammount = 0, initAmmount = 0;

		if(deposit){
			ammount = deposit->getAmmount();
			initAmmount = deposit->getInitAmmount();
		}

		Unit::displayUnitStats(ammountForeground, ammountBackground, ammount, initAmmount, mainPlayer == player && mainPlayerSelecting, Vector2(0, -10));
	}

	void Extractor::draw(){
		if(canDraw()){
			deposit->decrementAmmount();
			lastDrawTime = getTime();
		}
	}

	bool Extractor::canDraw(){return vb01::getTime() - lastDrawTime > drawRate && deposit->getAmmount() > 0;}
}
