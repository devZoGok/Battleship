#ifndef RESOURCE_DEPOSIT_H
#define RESOURCE_DEPOSIT_H

#include "gameObject.h"

namespace battleship{
	class ResourceDeposit : public GameObject{
		public:
			ResourceDeposit(Player*, int, vb01::Vector3, vb01::Quaternion);
			~ResourceDeposit();
		private:
	};
}

#endif
