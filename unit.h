#pragma once
#include "external/vb01/quaternion.h"
#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <util.h>
#include <quaternion.h>
#include <lineRenderer.h>

#include <SFML/Audio.hpp>

#include "gameManager.h"
#include "projectile.h"

namespace vb01{
	class Mesh;
	class Quad;
	class Node;
	class Camera;
}

namespace battleship{
    class Player;
		class Unit;
    
    struct Order {
        enum class TYPE {ATTACK, BUILD, MOVE, PATROL, LAUNCH};
			struct Target{
				Unit *unit = nullptr;
				vb01::Vector3 pos;

				Target(){}
				Target(Unit *u, vb01::Vector3 p) : unit(u), pos(p){}
			};

        TYPE type;
		vb01::LineRenderer::Line line;
        std::vector<Target> targets;

		Order(){}
		Order(TYPE t, vb01::LineRenderer::Line l, std::vector<Target> targ) : type(t), line(l), targets(targ){}
    };
    
    enum class MoveDir {LEFT, UP, FORW};
    enum class Corner {FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT};
    enum class UnitClass {VESSEL, ENGINEER, SAMPLE_BUILDING};
    enum class UnitType {UNDERWATER, SEA_LEVEL, HOVER, LAND, AIR, NONE = -1};
    
    class Unit {
    public:
        Unit(Player*, int, vb01::Vector3, vb01::Quaternion);
        ~Unit();
        virtual void update();
        virtual void blowUp();
        virtual void halt();
        void toggleSelection(bool);
        void setOrder(Order);
        void placeUnit(vb01::Vector3);
        void orientUnit(vb01::Quaternion);
        void addProjectile(Projectile*);
        std::vector<Projectile*> getProjectiles();
        virtual void addOrder(Order);
		virtual void reinitUnit();
		inline vb01::Vector3 getCorner(int i){return corners[i];}
        inline bool isSelected(){return selected;}
        inline bool isSelectable(){return selectable;}
        inline bool isDebuggable(){return debugging;}
        inline bool isWorking(){return working;}
        inline vb01::Vector2 getScreenPos(){return screenPos;}
        inline vb01::Vector3 getPos() {return pos;}
        inline vb01::Vector3* getPosPtr() {return &pos;}
		inline vb01::Quaternion getRot(){return rot;}
        inline float getLineOfSight() {return lineOfSight;}
        inline float getWidth() {return width;}
        inline float getHeight() {return height;}
        inline float getLength() {return length;}
        inline vb01::Model* getNode() {return model;}
        inline Player* getPlayer(){return player;}
        inline UnitType getType() {return type;}
        inline UnitClass getUnitClass() {return unitClass;}
        inline void toggleDebugging(bool d){this->debugging=d;}
        inline void takeDamage(int damage) {health -= damage;}
        inline int getId() {return id;}
        inline int getPlayerId() {return playerId;}
        inline vb01::Vector3 getDirVec() {return dirVec;}
        inline vb01::Vector3 getLeftVec() {return leftVec;}
        inline vb01::Vector3 getUpVec() {return upVec;}
    private:
        void updateScreenCoordinates();
        inline bool canDisplayOrderLine(){return vb01::getTime() - orderLineDispTime < orderVecDispLength;}

        const int orderVecDispLength = 2000;
        sf::SoundBuffer *selectionSfxBuffer;
        sf::Sound *selectionSfx = nullptr;
		vb01::Node *hpBackgroundNode = nullptr, *hpForegroundNode = nullptr;
    protected:
        Player *player;
        UnitClass unitClass;
        UnitType type;
		vb01::Vector2 screenPos;
        std::vector<Order> orders;
		vb01::Vector3 pos = vb01::Vector3(0, 0, 0), upVec = vb01::Vector3(0, 1, 0), dirVec = vb01::Vector3(0, 0, 1), leftVec = vb01::Vector3(1, 0, 0), corners[8];
		vb01::Quaternion rot = vb01::Quaternion::QUAT_W;
        int health, maxHealth, cost, id, playerId, lenHpBar = 200;
        s64 orderLineDispTime = 0;
		vb01::Model *model;
        bool selected = false, selectable, debugging = false, working = true;
        float lineOfSight, range, width, height, length;

        void removeOrder(int);
        void displayUnitStats(vb01::Node*, vb01::Node*, int, int, vb01::Vector2 = vb01::Vector2::VEC_ZERO);
		virtual void initProperties();
		virtual void destroyModel();
		virtual void initModel();
		virtual void destroySound();
		virtual void initSound();
		virtual void initUnitStats();
        virtual void executeOrders();
        virtual void attack(Order){}
        virtual void build(Order){}
        virtual void move(Order){}
        virtual void patrol(Order){}
        virtual void launch(Order){}
		vb01::Node* createBar(float, vb01::Vector4);
    };
}

#endif
