#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "gameManager.h"
#include "unit.h"

namespace battleship{
    class Player {
    public:
        Player(int, int, vb01::Vector3 = vb01::Vector3::VEC_ZERO);
        ~Player();
        void update();
		void issueOrder(Order::TYPE, std::vector<Order::Target>, bool);
		void deselectUnit(int);
		void deselectUnits();
        bool isThisPlayersUnit(Unit*);
		const inline std::vector<Unit*>& getSelectedUnits(){return selectedUnits;}
		inline Unit* getSelectedUnit(int i){return selectedUnits[i];}
		inline int getNumSelectedUnits(){return selectedUnits.size();}
		inline void selectUnit(Unit *u){selectedUnits.push_back(u);}
		inline void selectUnits(std::vector<Unit*> units){selectedUnits = units;}
        inline void addUnit(Unit *u){units.push_back(u);}
        inline Unit* getUnit(int i){return units[i];}
        inline std::vector<Unit*>& getUnits(){return units;}
        inline void setId(int i){id=i;}
        inline int getId(){return id;}
        inline int getNumberOfUnits(){return units.size();}
        inline int getFaction(){return faction;}
        inline int getSide(){return side;}
        inline vb01::Vector3 getSpawnPoint(){return spawnPoint;}
    private:
        int credits, faction, difficulty,side,id;
        std::vector<Unit*> units, selectedUnits;
        vb01::Vector3 spawnPoint;
    };
}

#endif
