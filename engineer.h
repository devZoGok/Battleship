#ifndef ENGINEER_H
#define ENGINEER_H

#include <util.h>

#include "vehicle.h"

namespace battleship{
	class Structure;

	class Engineer : public Vehicle{
		public:
			Engineer(Player*, int, vb01::Vector3, vb01::Quaternion, Unit::State);
			void build(Order);
		private:
			vb01::s64 lastIncrementTime = 0;
			int buildRate = 100;
	};
}

#endif
