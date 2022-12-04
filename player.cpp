#include "player.h"

using namespace vb01;
using namespace std;

namespace battleship{
    Player::Player(int difficulty, int faction, Vector3 spawnPoint) {
        this->difficulty = difficulty;
        this->faction = faction;
        this->spawnPoint=spawnPoint;
    }

    Player::~Player() {
    }

    void Player::update() {
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

    bool Player::isThisPlayersUnit(Unit *u) {
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
