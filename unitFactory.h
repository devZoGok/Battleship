#ifndef UNIT_FACTORY_H
#define UNIT_FACTORY_H

#include <vector.h>
#include <quaternion.h>

namespace battleship{
	class Unit;
	class Player;

	class UnitFactory{
		public:
			static Unit* createUnit(Player*, int, vb01::Vector3, vb01::Quaternion);
		private:
	};
}

#endif
