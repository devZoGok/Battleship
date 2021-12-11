#pragma once
#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <util.h>
#include <SFML/Audio.hpp>

#include "unitData.h"
#include "gameManager.h"
#include "projectile.h"

namespace vb01{
		class Mesh;
		class Node;
		class Camera;
}

namespace battleship{
    class Player;
    
    struct Order {
        enum class TYPE {ATTACK, MOVE, PATROL, LAUNCH};
        TYPE type;
        std::vector<vb01::Vector3*> targetPos;
    };
    
    enum class MoveDir {FORWARD, LEFT, RIGHT};
    
    enum class Corner {FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT};
    
    class Unit {
    public:
        Unit(Player*, vb01::Vector3, int);
        ~Unit();
        virtual void update();
        virtual void blowUp();
        //virtual void updateUnitGUIInfo(ICameraSceneNode*, vector3df, vector3df, vector3df);
        virtual void debug();
        virtual void halt();
        void toggleSelection(bool);
        void setOrder(Order);
        void placeUnit(vb01::Vector3);
        void orientUnit(vb01::Vector3);
        void addProjectile(Projectile*);
        float getCircleRadius();
				vb01::Vector3 getCorner(int);
        std::vector<Projectile*> getProjectiles();
        inline bool isSelected(){return selected;}
        inline bool isSelectable(){return selectable;}
        inline bool isDebuggable(){return debugging;}
        inline bool isWorking(){return working;}
        inline vb01::Vector2 getScreenPos(){return screenPos;}
        inline void addOrder(Order o){orders.push_back(o);}
        inline vb01::Vector3 getPos() {return pos;}
        inline vb01::Vector3* getPosPtr() {return &pos;}
        //inline ILightSceneNode* getLight() {return light;}
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
        //void updateScreenCoordinates(ICameraSceneNode*, vector3df, vector3df, vector3df);
        void displayUnitStats();
        inline int getNextPatrolPointId() {return patrolPointId == patrolPoints.size() - 1 ? 0 : patrolPointId + 1;}
        inline bool canDisplayOrderLine(){return vb01::getTime() - orderLineDispTime < orderVecDispLength;}
        //ILightSceneNode *light;
        const int orderVecDispLength = 2000;
        sf::SoundBuffer *selectionSfxBuffer;
        sf::Sound *selectionSfx = nullptr;
    protected:
        Player *player;
        MoveDir moveDir = MoveDir::FORWARD;
        //vb01::Material createLineMaterial();
        unitData::UNIT_TYPE type;
				vb01::Camera *cam = nullptr;
				vb01::Vector2 screenPos;
        std::vector<Order> orders;
        std::vector<vb01::Vector3> patrolPoints;
				vb01::Vector3 pos = vb01::Vector3(0, 0, 0), upVec = vb01::Vector3(0, 1, 0), dirVec = vb01::Vector3(0, 0, -1), leftVec = vb01::Vector3(1, 0, 0);
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
