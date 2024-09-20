#ifndef MINIMAP_BUTTON_H
#define MINIMAP_BUTTON_H

#include <button.h>

namespace vb01{
	class Node;
}

namespace battleship{
	class MinimapButton : public vb01Gui::Button{
		public:
			MinimapButton(vb01::Vector3, vb01::Vector2, std::string);
			~MinimapButton();
			void onClick();
			void update();
		private:
	};
}

#endif
