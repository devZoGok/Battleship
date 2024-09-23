#ifndef MINIMAP_BUTTON_H
#define MINIMAP_BUTTON_H

#include <button.h>

namespace vb01{
	class Node;
}

namespace battleship{
	class MinimapButton : public vb01Gui::Button{
		public:
			MinimapButton(vb01::Vector3 pos, vb01::Vector2 size, std::string ip) : vb01Gui::Button(pos, size, "minimap", "", -1, true, ip){}
			~MinimapButton(){}
			void onClick();
		private:
	};
}

#endif
