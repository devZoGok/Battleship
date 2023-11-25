#include "engineer.h"
#include "map.h"
#include "player.h"
#include "structure.h"

namespace battleship{
	using namespace vb01;

	Engineer::Engineer(Player *player, int id, Vector3 pos, Quaternion rot) : Vehicle(player, id, pos, rot){
	}

	void Engineer::build(Order order){
		if(pathPoints.empty()){
			if(!order.targets[0].unit){
				player->addUnit(order.targets[0].unit);
			}
			else {
				Structure *structure = (Structure*)order.targets[0].unit;

				if(structure->getBuildStatus() < 100 && canIncrementStructureStatus()){
					structure->incrementBuildStatus();
					lastIncrementTime = getTime();
				}
				else if(structure->getBuildStatus() >= 100)
					removeOrder(0);
			}
		}
		else{
			navigate(0.5 * Map::getSingleton()->getCellSize().x);

			if(type == UnitType::LAND)
				alignToSurface();
		}
	}
}
