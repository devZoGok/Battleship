#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <util.h>
#include <quaternion.h>
#include <lineRenderer.h>

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
    
    struct Order {
        enum class TYPE {ATTACK, BUILD, MOVE, GARRISON, EJECT, PATROL, LAUNCH};
			struct Target{
				Unit *unit = nullptr;
				vb01::Vector3 pos;

				Target(){}
				Target(Unit *u, vb01::Vector3 p = vb01::Vector3::VEC_ZERO) : unit(u), pos(p){}
			};

        TYPE type;
		vb01::LineRenderer::Line line;
		vb01::Vector3 direction;
        std::vector<Target> targets;

		Order(){}
		Order(TYPE t, vb01::LineRenderer::Line l, std::vector<Target> targ, vb01::Vector3 dir) : type(t), line(l), targets(targ), direction(dir){}
    };
    
    enum class MoveDir {LEFT, UP, FORW};
    enum class Corner {FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT};
    enum class UnitClass {WAR_MECH, ENGINEER, TRANSPORT, CARGO_SHIP, CARRIER, SUBMARINE, SAMPLE_STRUCTURE};
    enum class UnitType {UNDERWATER, SEA_LEVEL, HOVER, LAND, AIR, NONE = -1};
    
    class Unit : public GameObject{
    public:
		struct GarrisonSlot{
			Vehicle *vehicle = nullptr;
			vb01::Node *background, *foreground;
			vb01::Vector2 offset;

			GarrisonSlot(vb01::Node *bg, vb01::Node *fg, vb01::Vector2 off, Vehicle *v = nullptr) : background(bg), foreground(fg), offset(off), vehicle(v){}
		};

        Unit(Player*, int, vb01::Vector3, vb01::Quaternion);
        ~Unit();
        virtual void update();
        virtual void blowUp();
        virtual void halt();
		void updateGarrison(Vehicle*, bool);
        virtual void toggleSelection(bool);
        void setOrder(Order);
        std::vector<Projectile*> getProjectiles();
        virtual void addOrder(Order);
		virtual void reinit();
		inline const std::vector<GarrisonSlot>& getGarrisonSlots(){return garrisonSlots;}
		inline vb01::Vector3 getCorner(int i){return corners[i];}
        inline vb01::Vector2 getScreenPos(){return screenPos;}
        inline vb01::Vector3* getPosPtr() {return &pos;}
        inline float getLineOfSight() {return lineOfSight;}
        inline vb01::Model* getNode() {return model;}
        inline UnitType getType() {return type;}
        inline UnitClass getUnitClass() {return unitClass;}
        inline void takeDamage(int damage) {health -= damage;}
        inline int getPlayerId() {return playerId;}
    private:
        void updateScreenCoordinates();
		void init();
        inline bool canDisplayOrderLine(){return vb01::getTime() - orderLineDispTime < orderVecDispLength;}

        const int orderVecDispLength = 2000;
        sf::SoundBuffer *selectionSfxBuffer;
        sf::Sound *selectionSfx = nullptr;
		vb01::Node *hpBackgroundNode = nullptr, *hpForegroundNode = nullptr;
    protected:
        UnitClass unitClass;
        UnitType type;
		vb01::Vector2 screenPos;
        std::vector<Order> orders;
		vb01::Vector3 corners[8];
		sf::SoundBuffer *fireSfxBuffer;
		sf::Sound *fireSfx = nullptr;
        int health, maxHealth, cost, id, playerId, lenHpBar = 200, rateOfFire;
        s64 orderLineDispTime = 0, lastFireTime = 0;
        float lineOfSight, range;
		std::vector<GarrisonSlot> garrisonSlots;

        void removeOrder(int);
		virtual void initProperties();
		virtual void destroySound();
		virtual void initSound();
		virtual void initUnitStats();
        virtual void executeOrders();
        virtual void eject(Order);
        virtual void attack(Order){}
        virtual void garrison(Order){}
        virtual void build(Order){}
        virtual void move(Order){}
        virtual void patrol(Order){}
        virtual void launch(Order){}
		void removeBar(vb01::Node*);
		vb01::Node* createBar(vb01::Vector2, vb01::Vector2, vb01::Vector4);
        void displayUnitStats(vb01::Node*, vb01::Node*, int, int, vb01::Vector2 offset = vb01::Vector2::VEC_ZERO);
    };
}

#endif
