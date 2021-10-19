#include"Player.h"

using namespace game::core;

namespace game{
    namespace content{
        Player::Player(GameManager *gM, int difficulty, int faction,vector3df spawnPoint) {
            gameManager = gM;
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
