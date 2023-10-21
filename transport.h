#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "vehicle.h"

namespace battleship{
	class Player;

	class Transport : public Vehicle{
		public:
			Transport(Player*, int, vb01::Vector3, vb01::Quaternion);
		private:
	};
}

#endif
