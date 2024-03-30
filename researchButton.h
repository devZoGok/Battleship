#ifndef RESEARCH_BUTTON_H
#define RESEARCH_BUTTON_H

#include "unitButton.h"

namespace battleship{
	class ResearchButton : public UnitButton{
		public:
			ResearchButton(vb01::Vector2, vb01::Vector2, std::string, int, std::string, int, int);
			~ResearchButton();
			void onClick();
			void update();
		private:
			int techId;
			bool active = false;
			vb01::Node *overlay = nullptr;
	};
}

#endif
