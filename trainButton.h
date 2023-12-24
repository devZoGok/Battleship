#ifndef TRAIN_BUTTON_H
#define TRAIN_BUTTON_H

#include "unitButton.h"

namespace battleship{
	class TrainButton : public UnitButton{
		public:
			TrainButton(vb01::Vector2, vb01::Vector2, std::string, int, std::string, int, int);
			void onClick();
		private:
			int factoryId, trainableUnitId;
	};
}

#endif
