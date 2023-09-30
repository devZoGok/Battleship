#include "addResourceCommand.h"
#include "map.h"
#include "player.h"

namespace battleship{
	void AddResourceCommand::validate(){
		if(arguments.size() != 3)
			return;

		playerId = atoi(arguments[0].c_str());

		if(playerId != 0 && playerId != 1)
			return;

		resourceId = atoi(arguments[1].c_str());

		if(!(0 <= resourceId && resourceId <= 2))
			return;

		resourceAmmount = atoi(arguments[2].c_str());
	}

	void AddResourceCommand::addResource(){
		Player *player = Map::getSingleton()->getPlayer(playerId);

		switch(resourceId){
			case 0:
				player->addRefineds(resourceAmmount);
				break;
			case 1:
				player->addWealth(resourceAmmount);
				break;
			case 2:
				player->addResearch(resourceAmmount);
				break;
		}
	}

	void AddResourceCommand::execute(){
		AbstractCommand::handle();
		validate();
		addResource();
	}
}
