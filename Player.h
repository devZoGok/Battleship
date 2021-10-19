#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include<irrlicht.h>
#include<vector>
#include"GameManager.h"
#include"Unit.h"

namespace game{
    namespace content{
        class Player {
        public:
            Player(core::GameManager*, int, int,irr::core::vector3df=irr::core::vector3df(0,0,0));
            ~Player();
            void update();
            bool isThisPlayersUnit(Unit*);
            inline void addUnit(Unit *u){units.push_back(u);}
            inline Unit* getUnit(int i){return units[i];}
            inline std::vector<Unit*>& getUnits(){return units;}
            inline void setId(int i){id=i;}
            inline int getId(){return id;}
            inline int getNumberOfUnits(){return units.size();}
            inline int getFaction(){return faction;}
            inline int getSide(){return side;}
            inline irr::core::vector3df getSpawnPoint(){return spawnPoint;}
        private:
            int credits, faction, difficulty,side,id;
            std::vector<Unit*> units;
            irr::core::vector3df spawnPoint;
            core::GameManager *gameManager;
        };
    }
}

#endif
