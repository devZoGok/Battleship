#include <solUtil.h>

#include "player.h"
#include "resourceDeposit.h"

namespace battleship{
	using namespace vb01;
	using namespace gameBase;
	using namespace std;

    Player::Player(int diff, int fac, int i, Vector3 sp) : difficulty(diff), faction(fac), id(i), spawnPoint(sp) {
		//SOL_LUA_STATE.script("players[" + to_string(id + 1) + "] = Player:new({id = " + to_string(id + 1) + "})");
    }

    Player::~Player() {
    }

    void Player::update() {
		//SOL_LUA_STATE.script("players[" + to_string(id + 1) + "]:update()");

		for(Unit *u : units)
			u->update();

		for(ResourceDeposit *rd : resourceDeposits)
			rd->update();
    }

	void Player::issueOrder(Order::TYPE type, vector<Order::Target> targets, bool append){
		Order order;
		order.type = type;
		order.targets = targets;

		for(Unit *u : selectedUnits){
			if(append)
				u->addOrder(order);
			else
				u->setOrder(order);
		}
	}

    bool Player::isThisPlayersUnit(GameObject *u) {
        bool foundUnit = false;

        if (units.size() > 0) {
            for (int i = 0; i < units.size() && !foundUnit; i++)
                if (units[i] == u)
                    foundUnit = true;

            return foundUnit;
        } else
            return false;
    }

	void Player::deselectUnit(int i){
		selectedUnits[i]->toggleSelection(false);
		selectedUnits.erase(selectedUnits.begin() + i);
	}

	void Player::deselectUnits(){
		for(Unit *u : selectedUnits)
			u->toggleSelection(false);

		selectedUnits.clear();
	}
}
