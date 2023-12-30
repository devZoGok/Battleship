#ifndef BUILD_BUTTON_H
#define BUILD_BUTTON_H

#include "unitButton.h"

namespace battleship{
	class BuildButton : public UnitButton{
		public:
			BuildButton(vb01::Vector2, vb01::Vector2, int, std::string, int, std::string);
			void onClick();
		private:
			int structureId;
	};
}

#endif
