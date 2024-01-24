#ifndef RESOURCE_DEPOSIT_H
#define RESOURCE_DEPOSIT_H

#include "gameObject.h"

namespace battleship{
	class ResourceDeposit : public GameObject{
		public:
			ResourceDeposit(Player*, int, vb01::Vector3, vb01::Quaternion, int);
			~ResourceDeposit();
			inline int getAmmount(){return ammount;}
			inline int getInitAmmount(){return initAmmount;}
			inline void decrementAmmount(){ammount--;}
		private:
			int ammount, initAmmount;
	};
}

#endif
