#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "unit.h"

namespace vb01{
	class Node;
}

namespace battleship{
	class Structure : public Unit{
		public:
			Structure(Player*, int, vb01::Vector3, vb01::Quaternion, int = 0, Unit::State = Unit::State::STAND_GROUND);
			~Structure();
			virtual void update();
			inline int getBuildStatus(){return buildStatus;}
			inline void incrementBuildStatus(){buildStatus++;}
		private:
			int buildStatus = 0;
		protected:
			vb01::Node *buildStatusBackground = nullptr, *buildStatusForeground = nullptr;
	};
}

#endif
