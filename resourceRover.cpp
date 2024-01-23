#include "resourceRover.h"
#include "map.h"
#include "player.h"
#include "activeGameState.h"

#include <stateManager.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	ResourceRover::ResourceRover(Player *player, int id, Vector3 pos, Quaternion rot) : 
		Vehicle(player, id, pos, rot), 
		capacity(300),
		loadRate(10) 
	{
		Vector2 size = Vector2(lenHpBar, 10);
		loadBackground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 0, 0, 1));
		loadForeground = Unit::createBar(Vector2::VEC_ZERO, size,  Vector4(0, 1, 1, 1));
	}

	ResourceRover::~ResourceRover(){
		removeBar(loadForeground);
		removeBar(loadBackground);
	}

	void ResourceRover::supply(Order order){
		float minDist = .6 * Map::getSingleton()->getCellSize().x;

		if(!pathPoints.empty())
			navigate(minDist);
		else{
			vector<Unit*> units = player->getUnits(), extractors, refineries;

			for(Unit *unit : units){
				if(unit->getUnitClass() == UnitClass::EXTRACTOR)
					extractors.push_back(unit);
				else if(unit->getUnitClass() == UnitClass::REFINERY)
					refineries.push_back(unit);
			}

			nearestExtractor = getClosestUnit(extractors);
			nearestRefinery = getClosestUnit(refineries);

			if(nearestExtractor->getPos().getDistanceFrom(pos) <= minDist){
				if(canLoad()){
					load++;
					lastLoadTime = getTime();
				}
				else if(load == capacity && nearestRefinery)
					preparePathpoints(order, nearestRefinery->getPos());
			}
			else if(nearestRefinery->getPos().getDistanceFrom(pos) <= minDist){
				if(canUnload()){
					load--;
					lastLoadTime = getTime();
				}
				else if(load == 0 && nearestExtractor)
					preparePathpoints(order, nearestExtractor->getPos());
			}
		}
	}

	void ResourceRover::update(){
		Vehicle::update();

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		vector<Player*> selectingPlayers = getSelectingPlayers();
		bool mainPlayerSelecting = (activeState && find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());

		Unit::displayUnitStats(loadForeground, loadBackground, load, capacity, mainPlayer == player && mainPlayerSelecting, Vector2(0, -10));
	}

	Unit* ResourceRover::getClosestUnit(vector<Unit*> units){
		if(units.empty()) return nullptr;

		int minDistId = 0;

		for(int i = 0; i < units.size(); i++)
			if(units[minDistId]->getPos().getDistanceFrom(pos) > units[i]->getPos().getDistanceFrom(pos))
				minDistId = i;

		return units[minDistId];
	}
}
