#ifndef UNIT_BUTTON_H
#define UNIT_BUTTON_H

#include <button.h>

namespace battleship{
	class Unit;

	class UnitButton : public vb01Gui::Button{
		public:
			UnitButton(vb01::Vector2, vb01::Vector2, std::string, std::string, int, std::string, int);
		protected:
			int unitId;

			std::vector<Unit*> getUnits(int);
	};
}

#endif
