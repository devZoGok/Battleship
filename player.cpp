#include "player.h"

using namespace game::core;
using namespace vb01;

namespace game{
    namespace content{
        Player::Player(int difficulty, int faction, Vector3 spawnPoint) {
            this->difficulty = difficulty;
            this->faction = faction;
            this->spawnPoint=spawnPoint;
        }

        Player::~Player() {
        }

        void Player::update() {
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
    }
}
