#include "resourceRover.h"
#include "map.h"
#include "player.h"

namespace battleship{
	using namespace std;
	using namespace vb01;

	ResourceRover::ResourceRover(Player *player, int id, Vector3 pos, Quaternion rot) : 
		Vehicle(player, id, pos, rot), 
		capacity(300),
		loadRate(100) {}

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
				else if(!canLoad() && nearestRefinery)
					preparePathpoints(order, nearestRefinery->getPos());
			}
			else if(nearestRefinery->getPos().getDistanceFrom(pos) <= minDist){
				if(canUnload()){
					load--;
					lastLoadTime = getTime();
				}
				else if(!canUnload() && nearestExtractor)
					preparePathpoints(order, nearestExtractor->getPos());
			}
		}
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
