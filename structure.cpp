#include "structure.h"
#include "activeGameState.h"

#include <stateManager.h>

#include <node.h>

using namespace gameBase;
using namespace vb01;
using namespace std;

namespace battleship{
	Structure::Structure(Player *player, int id, Vector3 pos, Quaternion rot, int bldSt) : buildStatus(bldSt), Unit(player, id, pos, rot){
		Vector2 size = Vector2(lenHpBar, 10);
		buildStatusBackground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 0, 1));
		buildStatusForeground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 1, 1));
	}

	Structure::~Structure(){
		removeBar(buildStatusForeground);
		removeBar(buildStatusBackground);
		Unit::~Unit();
	}

	void Structure::update(){
		Unit::update();

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		vector<Player*> selectingPlayers = getSelectingPlayers();
		bool mainPlayerSelecting = (activeState && find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());

		if(buildStatus < 100)
			Unit::displayUnitStats(buildStatusForeground, buildStatusBackground, buildStatus, 100, mainPlayer == player && mainPlayerSelecting, Vector2(0, -10));
		else{
			buildStatusBackground->setVisible(false);
			buildStatusForeground->setVisible(false);
		}
	}
}
