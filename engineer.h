#ifndef ENGINEER_H
#define ENGINEER_H

#include <util.h>

#include "vehicle.h"

namespace vb01{
	class Node;
}

namespace battleship{
	class Structure;

	class Engineer : public Vehicle{
		public:
			Engineer(Player*, int, vb01::Vector3, vb01::Quaternion, Unit::State);
			~Engineer();
			void update();
		private:
			vb01::s64 lastIncrementTime = 0;
			int hackStatus = 0;
			vb01::Node *hackStatusBackground = nullptr, *hackStatusForeground = nullptr;

			void build(Order);
			void hack(Order);
	};
}

#endif
