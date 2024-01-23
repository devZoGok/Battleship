#ifndef RESOURCE_ROVER_H
#define RESOURCE_ROVER_H

#include "vehicle.h"

namespace battleship{
	class ResourceRover : public Vehicle{
		public:
			ResourceRover(Player*, int, vb01::Vector3, vb01::Quaternion);
		private:
	};
}

#endif
