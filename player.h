#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "gameManager.h"
#include "trader.h"
#include "unit.h"

namespace battleship{
	class ResourceDeposit;
	class Projectile;
	class Unit;

	enum class ResourceType{REFINEDS, WEALTH, RESEARCH};

    class Player {
    public:
        Player(int, int, int, vb01::Vector3, bool = true, vb01::Vector3 = vb01::Vector3::VEC_ZERO, std::string = "");
        ~Player();
        void update();
		void issueOrder(Order::TYPE, vb01::Vector3, std::vector<Order::Target>, bool);
		void removeUnit(Unit*);
		void removeUnit(int);
		void removeResourceDeposit(int);
		void removeProjectile(int);
		void removeProjectile(Projectile*);
        bool isThisPlayersUnit(GameObject*);
		void selectUnits(std::vector<Unit*>);
		std::vector<Unit*> getUnitsById(int, int = -1);
		std::vector<Unit*> getUnitsByClass(UnitClass, int = -1);
		void addTechnology(int);
		int getResource(ResourceType);
		void updateResource(ResourceType, int, bool);
		inline Trader* getTrader(){return trader;}
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
		inline void addProjectile(Projectile *proj){projectiles.push_back(proj);}
		inline int getNumProjectiles(){return projectiles.size();}
        inline std::vector<Projectile*>& getProjectiles(){return projectiles;}
        inline Unit* getUnit(int i){return units[i];}
        inline std::vector<Unit*>& getUnits(){return units;}
        inline void setTeam(int t){team = t;}
        inline int getTeam(){return team;}
        inline int getNumUnits(){return units.size();}
        inline int getFaction(){return faction;}
        inline vb01::Vector3 getSpawnPoint(){return spawnPoint;}
		inline bool isCpuPlayer(){return cpuPlayer;}
		inline int getNumVehiclesBuilt(){return vehiclesBuilt;}
		inline int getNumVehiclesDestroyed(){return vehiclesDestroyed;}
		inline int getNumVehiclesLost(){return vehiclesLost;}
		inline int getNumStructuresBuilt(){return structuresBuilt;}
		inline int getNumStructuresDestroyed(){return structuresDestroyed;}
		inline int getNumStructuresLost(){return structuresLost;}
		inline void incVehiclesBuilt(){vehiclesBuilt++;}
		inline void incVehiclesDestroyed(){vehiclesDestroyed++;}
		inline void incVehiclesLost(){vehiclesLost++;}
		inline void incStructuresBuilt(){structuresBuilt++;}
		inline void incStructuresDestroyed(){structuresDestroyed++;}
		inline void incStructuresLost(){structuresLost++;}
		inline vb01::Vector3 getColor(){return color;}
		inline std::string getName(){return name;}
		inline vb01::Material* getColorMaterial(){return colorMaterial;}
		inline std::vector<int> getTechnologies(){return technologies;}
    private:
		bool cpuPlayer = false;
		std::vector<int> technologies;
		int luaPlayerId;
        int refineds = 0, wealth = 0, research = 0;
		int faction, difficulty, team;
		int vehiclesBuilt = 0, vehiclesDestroyed = 0, vehiclesLost = 0;
		int structuresBuilt = 0, structuresDestroyed = 0, structuresLost = 0;
		Trader *trader = nullptr; 
		std::string name;
        std::vector<Unit*> units, selectedUnits;
		std::vector<Projectile*> projectiles;
		std::vector<ResourceDeposit*> resourceDeposits;
        vb01::Vector3 spawnPoint, color;
		vb01::Material *colorMaterial = nullptr;

		int getOrderLineId(Order::TYPE, vb01::Vector3, vb01::Vector3);
    };
}

#endif
