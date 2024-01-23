#ifndef RESOURCE_ROVER_H
#define RESOURCE_ROVER_H

#include "vehicle.h"

#include <util.h>

namespace battleship{
	class Structure;

	class ResourceRover : public Vehicle{
		public:
			ResourceRover(Player*, int, vb01::Vector3, vb01::Quaternion);
			~ResourceRover();
			void update();
		private:
			void supply(Order);
			Unit* getClosestUnit(std::vector<Structure*>);
			inline bool canLoad(){return vb01::getTime() - lastLoadTime > loadRate && load < capacity;}
			inline bool canUnload(){return vb01::getTime() - lastLoadTime > loadRate && load > 0;}

			vb01::s64 lastLoadTime = 0;
			int load = 0, capacity, loadRate;
			Unit *nearestExtractor = nullptr, *nearestRefinery = nullptr;
			vb01::Node *loadBackground = nullptr, *loadForeground = nullptr;
	};
}

#endif
