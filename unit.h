#pragma once
#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <util.h>
#include <quaternion.h>
#include <lineRenderer.h>

#include <SFML/Audio.hpp>

#include "unitData.h"
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
        enum class TYPE {ATTACK, MOVE, PATROL, LAUNCH};
				struct Target{
						Unit *unit = nullptr;
						vb01::Vector3 *pos = nullptr;

						Target(){}

						Target(Unit *unit, vb01::Vector3 *pos){
								this->unit = unit;
								this->pos = pos;
						}
				};

        TYPE type;
				vb01::LineRenderer::Line line;
        std::vector<Target> targets;
    };
    
    enum class MoveDir {FORWARD, LEFT, RIGHT};
    
    enum class Corner {FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT};
    
    class Unit {
    public:
        Unit(Player*, vb01::Vector3, int);
        ~Unit();
        virtual void update();
        virtual void blowUp();
        virtual void halt();
        void toggleSelection(bool);
        void setOrder(Order);
        void placeUnit(vb01::Vector3);
        void orientUnit(vb01::Vector3);
        void addProjectile(Projectile*);
        float getCircleRadius();
				vb01::Vector3 getCorner(int);
        std::vector<Projectile*> getProjectiles();
        inline void addOrder(Order o){orders.push_back(o);}
        inline bool isSelected(){return selected;}
        inline bool isSelectable(){return selectable;}
        inline bool isDebuggable(){return debugging;}
        inline bool isWorking(){return working;}
        inline vb01::Vector2 getScreenPos(){return screenPos;}
        inline vb01::Vector3 getPos() {return pos;}
        inline vb01::Vector3* getPosPtr() {return &pos;}
        inline float getLineOfSight() {return lineOfSight;}
        inline float getWidth() {return width;}
        inline float getHeight() {return height;}
        inline float getLength() {return length;}
        inline vb01::Model* getNode() {return model;}
        inline Player* getPlayer(){return player;}
        inline unitData::UNIT_TYPE getType() {return type;}
        inline void toggleDebugging(bool d){this->debugging=d;}
        inline void takeDamage(int damage) {health -= damage;}
        inline int getId() {return id;}
        inline int getPlayerId() {return playerId;}
        inline vb01::Vector3 getDirVec() {return dirVec;}
        inline vb01::Vector3 getLeftVec() {return leftVec;}
        inline vb01::Vector3 getUpVec() {return upVec;}
    private:
        void updateScreenCoordinates();
        void displayUnitStats();
        inline int getNextPatrolPointId(int numPoints) {return patrolPointId == numPoints - 1 ? 0 : patrolPointId + 1;}
        inline bool canDisplayOrderLine(){return vb01::getTime() - orderLineDispTime < orderVecDispLength;}

        const int orderVecDispLength = 2000;
        sf::SoundBuffer *selectionSfxBuffer;
        sf::Sound *selectionSfx = nullptr;
				vb01::Quad *hpBackground = nullptr, *hpForeground = nullptr;
				vb01::Node *hpBackgroundNode = nullptr, *hpForegroundNode = nullptr;
				int lenHpBar = 200;
    protected:
        Player *player;
        MoveDir moveDir = MoveDir::FORWARD;
        unitData::UNIT_TYPE type;
				vb01::Vector2 screenPos;
        std::vector<Order> orders;
				vb01::Vector3 pos = vb01::Vector3(0, 0, 0), upVec = vb01::Vector3(0, 1, 0), dirVec = vb01::Vector3(0, 0, -1), leftVec = vb01::Vector3(1, 0, 0);
				vb01::Quaternion rot = vb01::Quaternion::QUAT_W;
        int health, cost, id, patrolPointId = 0, playerId;
        s64 orderLineDispTime = 0;
				vb01::Model *model;
        bool selected = false, selectable, debugging = false, working = true;
        float lineOfSight, speed, maxTurnAngle, range, width, height, length;

        void removeOrder(int);
        virtual void executeOrders();
        virtual void attack(Order);
        virtual void move(Order, float = 0.);
        virtual void patrol(Order);
        virtual void launch(Order);
        virtual void turn(float);
        virtual void advance(float);
        void drawCuboid();
    };
}

#endif
