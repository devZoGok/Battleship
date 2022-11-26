#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "unit.h"

namespace battleship{
	class Structure : public Unit{
		public:
			Structure(Player*, int, vb01::Vector3, vb01::Quaternion, int = 0);
			inline int getBuildStatus(){return buildStatus;}
			inline void incrementBuildStatus(){buildStatus++;}
		private:
			int buildStatus = 0;
	};
}

#endif
