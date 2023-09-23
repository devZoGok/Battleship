#ifndef UNIT_BUTTON_H
#define UNIT_BUTTON_H

#include <button.h>

namespace battleship{
	class Unit;

	class UnitButton : public vb01Gui::Button{
		public:
			UnitButton(vb01::Vector2 pos, vb01::Vector2 size, std::string name, std::string fontPath, int trigger, std::string imagePath) : Button(pos, size, name, fontPath, trigger, true, imagePath){}
		protected:
			std::vector<Unit*> units;
	};
}

#endif
