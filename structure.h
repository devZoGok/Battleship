#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "unit.h"

namespace battleship{
	class Structure : public Unit{
		public:
			Structure(Player*, int, vb01::Vector3, vb01::Quaternion);
		private:
	};
}

#endif
