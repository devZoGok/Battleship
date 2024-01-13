#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <util.h>
#include <quaternion.h>
#include <lineRenderer.h>

#include <solUtil.h>

#include "gameManager.h"
#include "gameObject.h"
#include "projectile.h"

namespace sf{
	class SoundBuffer;
	class Sound;
}

namespace vb01{
	class Mesh;
	class Quad;
	class Node;
	class Camera;
}

namespace battleship{
    class Player;
	class Unit;
	class Vehicle;
	class Factory;
	class Cruiser;
	class Transport;
	class PointDefense;
	class Engineer;
    
    struct Order {
        enum class TYPE {ATTACK, BUILD, MOVE, GARRISON, EJECT, PATROL, LAUNCH};
			struct Target{
				Unit *unit = nullptr;
				vb01::Vector3 pos;

				Target(){}
				Target(Unit *u, vb01::Vector3 p = vb01::Vector3::VEC_ZERO) : unit(u), pos(p){}
			};

        TYPE type;
		int lineId = -1;
		vb01::Vector3 direction;
        std::vector<Target> targets;

		Order(){}
		Order(TYPE t, std::vector<Target> targ, vb01::Vector3 dir, int lid = -1) : type(t), lineId(lid), targets(targ), direction(dir){}
    };
    
    enum class MoveDir {LEFT, UP, FORW};
    enum class Corner {FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT};
    enum class UnitClass {WAR_MECH, TANK, ARTILLERY, ENGINEER, TRANSPORT, CARGO_SHIP, CRUISER, CARRIER, SUBMARINE, LAND_FACTORY, NAVAL_FACTORY, MARKET, LAB, POINT_DEFENSE};
    enum class UnitType {UNDERWATER, SEA_LEVEL, HOVER, LAND, AIR, NONE = -1};
    
    class Unit : public GameObject{
    public:
		struct GarrisonSlot{
			Vehicle *vehicle = nullptr;
			int category;
			vb01::Node *background, *foreground;
			vb01::Vector2 offset;

			GarrisonSlot(vb01::Node *bg, vb01::Node *fg, vb01::Vector2 off, int cat, Vehicle *v = nullptr) : background(bg), foreground(fg), offset(off), category(cat), vehicle(v){}
		};

        Unit(Player*, int, vb01::Vector3, vb01::Quaternion);
        virtual ~Unit();
        virtual void update();
        virtual void blowUp();
        virtual void halt();
		void updateGarrison(Vehicle*, bool);
        virtual void select();
        void setOrder(Order);
        std::vector<Projectile*> getProjectiles();
        virtual void addOrder(Order);
		virtual void reinit();
		bool canGarrison(Vehicle*);
		inline Engineer* toEngineer(){return (Engineer*)this;}
		inline Transport* toTransport(){return (Transport*)this;}
		inline Factory* toFactory(){return (Factory*)this;}
		inline Cruiser* toCruiser(){return (Cruiser*)this;}
		inline PointDefense* toPointDefense(){return (PointDefense*)this;}
		inline int getNumGarrisonSlots(){return garrisonSlots.size();}
		inline const std::vector<GarrisonSlot>& getGarrisonSlots(){return garrisonSlots;}
        inline vb01::Vector3* getPosPtr() {return &pos;}
        inline float getLineOfSight() {return lineOfSight;}
        inline vb01::Model* getNode() {return model;}
        inline UnitType getType() {return type;}
        inline UnitClass getUnitClass() {return unitClass;}
        inline void takeDamage(int damage) {health -= damage;}
        inline int getPlayerId() {return playerId;}
		inline int getHealth(){return health;}
		inline bool isVehicle(){return gameBase::generateView()["units"]["isVehicle"][id + 1];}
		inline bool isTargetToTheRight(vb01::Vector3 dir, vb01::Vector3 lv){return lv.getAngleBetween(dir) > vb01::PI / 2;}
    private:
		void renderOrderLine(bool);
        void updateScreenCoordinates();
		void init();
        inline bool canDisplayOrderLine(){return vb01::getTime() - orderLineDispTime < orderVecDispLength;}

        const int orderVecDispLength = 2000, DEATH_HP = 0;
        sf::SoundBuffer *selectionSfxBuffer;
        sf::Sound *selectionSfx = nullptr;
		vb01::Node *hpBackgroundNode = nullptr, *hpForegroundNode = nullptr;
    protected:
        UnitClass unitClass;
        UnitType type;
        std::vector<Order> orders;
		sf::SoundBuffer *fireSfxBuffer;
		sf::Sound *fireSfx = nullptr;
        int health, damage, maxHealth, cost, id, playerId, lenHpBar = 200, rateOfFire;
        s64 orderLineDispTime = 0, lastFireTime = 0;
        float lineOfSight, range;
		std::vector<GarrisonSlot> garrisonSlots;

		std::vector<Player*> getSelectingPlayers();
        void removeOrder(int);
		virtual void initProperties();
		virtual void destroySound();
		virtual void initSound();
		virtual void initUnitStats();
        virtual void executeOrders();
        virtual void eject(Order);
        virtual void attack(Order);
        virtual void garrison(Order){}
        virtual void build(Order){}
        virtual void move(Order){}
        virtual void patrol(Order){}
        virtual void launch(Order){}
		float calculateRotation(vb01::Vector3, float, float);
		virtual void fire();
		void removeBar(vb01::Node*);
		vb01::Node* createBar(vb01::Vector2, vb01::Vector2, vb01::Vector4);
        void displayUnitStats(vb01::Node*, vb01::Node*, int, int, bool, vb01::Vector2 offset = vb01::Vector2::VEC_ZERO);
		inline bool canFire(){return vb01::getTime() - lastFireTime > rateOfFire;}
    };
}

#endif
