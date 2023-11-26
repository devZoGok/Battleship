#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "unit.h"

namespace vb01{
	class Node;
}

namespace battleship{
	class Structure : public Unit{
		public:
			Structure(Player*, int, vb01::Vector3, vb01::Quaternion, int = 0);
			~Structure();
			inline int getBuildStatus(){return buildStatus;}
			inline void incrementBuildStatus(){buildStatus++;}
			virtual void update();
		private:
			int buildStatus = 0;
		protected:
			vb01::Node *buildStatusBackground = nullptr, *buildStatusForeground = nullptr;
	};
}

#endif
