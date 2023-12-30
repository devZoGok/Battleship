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
        Player(int, int, int, vb01::Vector3, bool = true, vb01::Vector3 = vb01::Vector3::VEC_ZERO, std::string = "");
        ~Player();
        void update();
		void issueOrder(Order::TYPE, vb01::Vector3, std::vector<Order::Target>, bool);
		void removeUnit(Unit*);
		void removeUnit(int);
		void removeResourceDeposit(int);
        bool isThisPlayersUnit(GameObject*);
		std::vector<Unit*> getSelectedUnitsByClass(UnitClass);
		void selectUnits(std::vector<Unit*>);
		inline void deselectUnits(){selectedUnits.clear();}
		inline Unit* getSelectedUnit(int id){return selectedUnits[id];}
		inline std::vector<Unit*> getSelectedUnits(){return selectedUnits;}
		inline void deselectUnit(int i){selectedUnits.erase(selectedUnits.begin() + i);}
		inline int getNumSelectedUnits(){return getSelectedUnits().size();}
		inline void selectUnit(Unit *u){selectUnits(std::vector<Unit*>{u});}
        inline void addUnit(Unit *u){units.push_back(u);}
		inline std::vector<ResourceDeposit*>& getResourceDeposits(){return resourceDeposits;}
        inline void addResourceDeposit(ResourceDeposit *rd){resourceDeposits.push_back(rd);}
		inline int getNumResourceDeposits(){return resourceDeposits.size();}
        inline Unit* getUnit(int i){return units[i];}
        inline std::vector<Unit*>& getUnits(){return units;}
        inline void setTeam(int t){team = t;}
        inline int getTeam(){return team;}
        inline int getNumUnits(){return units.size();}
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
		inline int getNumVehiclesBuilt(){return vehiclesBuilt;}
		inline int getNumVehiclesDestroyed(){return vehiclesDestroyed;}
		inline int getNumVehiclesLost(){return vehiclesLost;}
		inline int getNumStructuresBuilt(){return structuresBuilt;}
		inline int getNumStructuresDestroyed(){return structuresDestroyed;}
		inline int getNumStructuresLost(){return structuresLost;}
		inline void setNumVehiclesBuilt(int vb){vehiclesBuilt = vb;}
		inline void setNumVehiclesDestroyed(int vd){vehiclesDestroyed = vd;}
		inline void setNumVehiclesLost(int vl){vehiclesLost = vl;}
		inline void setNumStructuresBuilt(int sb){structuresBuilt = sb;}
		inline void setNumStructuresDestroyed(int sd){structuresDestroyed = sd;}
		inline void setNumStructuresLost(int sl){structuresLost = sl;}
		inline vb01::Vector3 getColor(){return color;}
		inline std::string getName(){return name;}
    private:
		bool cpuPlayer = false;
        int refineds = 0, wealth = 0, research = 0, faction, difficulty, team, luaPlayerId, vehiclesBuilt = 0, vehiclesDestroyed = 0, vehiclesLost = 0, structuresBuilt = 0, structuresDestroyed = 0, structuresLost = 0;
		std::string name;
        std::vector<Unit*> units, selectedUnits;
		std::vector<Projectile*> projectiles;
		std::vector<ResourceDeposit*> resourceDeposits;
        vb01::Vector3 spawnPoint, color;

		int getOrderLineId(Order::TYPE, vb01::Vector3, vb01::Vector3);
    };
}

#endif
