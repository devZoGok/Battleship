#ifndef POINT_DEFENSE_H
#define POINT_DEFENSE_H

#include "structure.h"

namespace battleship{
	class PointDefense : public Structure{
		public:
			PointDefense(Player*, int, vb01::Vector3, vb01::Quaternion, int);
		private:
			vb01::Vector3 turretDir;

			void attack(Order);
			void rotateTurret(float);
	};
}

#endif
