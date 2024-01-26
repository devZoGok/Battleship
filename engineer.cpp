#include "engineer.h"
#include "map.h"
#include "player.h"
#include "structure.h"

#include <solUtil.h>

namespace battleship{
	using namespace vb01;
	using namespace gameBase;

	Engineer::Engineer(Player *player, int id, Vector3 pos, Quaternion rot) : Vehicle(player, id, pos, rot){}

	void Engineer::build(Order order){
		if(pathPoints.empty()){
			if(!order.targets[0].unit)
				player->addUnit(order.targets[0].unit);
			else {
				Structure *structure = (Structure*)order.targets[0].unit;
				sol::table targTable = generateView()["units"][order.targets[0].unit->getId()];
				int costRate = (int)targTable["cost"] / 100, buildRate = (int)targTable["buildTime"] / 100;

				if(structure->getBuildStatus() < 100 && player->getRefineds() >= costRate && getTime() - lastIncrementTime > buildRate){
					structure->incrementBuildStatus();
					player->setRefineds(player->getRefineds() - costRate);
					lastIncrementTime = getTime();
				}
				else if(structure->getBuildStatus() >= 100){
					removeOrder(0);
					player->incStructuresBuilt();
				}
			}
		}
		else{
			navigate(0.5 * Map::getSingleton()->getCellSize().x);

			if(type == UnitType::LAND)
				alignToSurface();
		}
	}
}
