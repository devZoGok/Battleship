#ifndef BUILD_BUTTON_H
#define BUILD_BUTTON_H

#include "unitButton.h"

namespace battleship{
	class BuildButton : public UnitButton{
		public:
			BuildButton(vb01::Vector2, vb01::Vector2, std::string, int, std::string, int, int);
			void onClick();
		private:
			int slotId;
	};
}

#endif
