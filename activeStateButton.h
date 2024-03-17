#ifndef ACTIVE_STATE_BUTTON_H
#define ACTIVE_STATE_BUTTON_H

#include <button.h>

namespace battleship{
	class ActiveStateButton : public vb01Gui::Button{
		public:
			ActiveStateButton(vb01::Vector2, vb01::Vector2, std::string, int = -1, std::string = "");
			void onClick();
		private:
	};
}

#endif
