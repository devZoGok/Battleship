#ifndef TRADING_SCREEN_BUTTON_H
#define TRADING_SCREEN_BUTTON_H

#include "activeStateButton.h"

namespace battleship{
	class TradingScreenButton : public ActiveStateButton{
		public:
			TradingScreenButton(vb01::Vector3, vb01::Vector2, int, std::string, std::string, int, std::string);
			void onClick();
		private:
			int playerId;
	};
}

#endif
