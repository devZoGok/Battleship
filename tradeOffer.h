#ifndef TRADE_OFFER_H
#define TRADE_OFFER_H

#include "player.h"

namespace battleship{
	struct TradeOffer{
		Player *initPlayer = nullptr, *recPlayer = nullptr;
		int initRefineds = 0, recRefineds = 0, initWealth = 0, recWealth = 0, initResearch = 0, recResearch = 0;
		bool initPlAgreed = true, recPlAgreed = false;

		TradeOffer(Player *ip, Player *rp, int ir, int rr, int iw, int rw, int it, int rt) : 
			initPlayer(ip), 
			recPlayer(rp), 
			initRefineds(ir), 
			recRefineds(rr), 
			initWealth(iw), 
			recWealth(rw), 
			initResearch(it), 
			recResearch(rr)
		{}
	};
}

#endif
