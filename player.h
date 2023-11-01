#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "gameManager.h"
#include "unit.h"

namespace battleship{
	class ResourceDeposit;
	class Projectile;

    class Player {
    public:
        Player(int, int, int, bool = true, vb01::Vector3 = vb01::Vector3::VEC_ZERO);
        ~Player();
        void update();
		void issueOrder(Order::TYPE, vb01::Vector3, std::vector<Order::Target>, bool);
		void deselectUnits();
		void removeUnit(Unit*);
		void removeUnit(int);
        bool isThisPlayersUnit(GameObject*);
		std::vector<Unit*> getSelectedUnitsByClass(UnitClass);
		std::vector<Unit*> getSelectedUnits();
		Unit* getSelectedUnit(int);
		void selectUnits(std::vector<Unit*>);
		inline void deselectUnit(int i){getSelectedUnit(i)->toggleSelection(false);}
		inline int getNumSelectedUnits(){return getSelectedUnits().size();}
		inline void selectUnit(Unit *u){u->toggleSelection(true);}
        inline void addUnit(Unit *u){units.push_back(u);}
		inline std::vector<ResourceDeposit*>& getResourceDeposits(){return resourceDeposits;}
        inline void addResourceDeposit(ResourceDeposit *rd){resourceDeposits.push_back(rd);}
        inline Unit* getUnit(int i){return units[i];}
        inline std::vector<Unit*>& getUnits(){return units;}
        inline void setTeam(int t){team = t;}
        inline int getTeam(){return team;}
        inline int getNumberOfUnits(){return units.size();}
        inline int getFaction(){return faction;}
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
		inline bool isCpuPlayer(){return cpuPlayer;}
    private:
		bool cpuPlayer = false;
        int refineds = 0, wealth = 0, research = 0, faction, difficulty, team, luaPlayerId;
        std::vector<Unit*> units;
		std::vector<Projectile*> projectiles;
		std::vector<ResourceDeposit*> resourceDeposits;
        vb01::Vector3 spawnPoint;

		int getOrderLineId(Order::TYPE, vb01::Vector3, vb01::Vector3);
		void syncLuaPlayer();
    };
}

#endif
