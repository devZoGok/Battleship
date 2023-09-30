#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "gameManager.h"
#include "unit.h"

namespace battleship{
    class Player {
    public:
        Player(int, int, int, vb01::Vector3 = vb01::Vector3::VEC_ZERO);
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
		inline int getRefineds(){return refineds;}
		inline void setRefineds(int ref){this->refineds = ref;}
		inline void addRefineds(int ref){this->refineds += ref;}
		inline int getWealth(){return wealth;}
		inline void setWealth(int w){this->wealth = w;}
		inline void addWealth(int w){this->wealth += w;}
		inline int getResearch(){return research;}
		inline void setResearch(int r){this->research = r;}
		inline void addResearch(int r){this->research += r;}
    private:
        int refineds = 0, wealth = 0, research = 0, faction, difficulty,side,id;
        std::vector<Unit*> units, selectedUnits;
        vb01::Vector3 spawnPoint;
    };
}

#endif
