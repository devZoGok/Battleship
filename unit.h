#pragma once
#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <irrlicht.h>
#include <SFML/Audio.hpp>

#include "unitData.h"
#include "gameManager.h"
#include "projectile.h"
#include "util.h"

using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace game::content::unitData;

namespace game{
    namespace content{
        class Player;
        
        struct Order {
            enum class TYPE {ATTACK, MOVE, PATROL, LAUNCH};
            TYPE type;
            std::vector<vector3df*> targetPos;
        };
        
        enum class MoveDir {FORWARD, LEFT, RIGHT};
        
        enum class Corner {FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT};
        
        class Unit {
        public:
            Unit(Player*,vector3df, int);
            ~Unit();
            virtual void update();
            virtual void blowUp();
            virtual void updateUnitGUIInfo(ICameraSceneNode*, vector3df, vector3df, vector3df);
            virtual void debug();
            virtual void halt();
            void toggleSelection(bool);
            void setOrder(Order);
            void placeUnit(vector3df);
            void orientUnit(vector3df);
            void addProjectile(Projectile*);
            float getCircleRadius();
            vector3df getCorner(int);
            std::vector<Projectile*> getProjectiles();
            inline bool isSelected(){return selected;}
            inline bool isSelectable(){return selectable;}
            inline bool isDebuggable(){return debugging;}
            inline bool isWorking(){return working;}
            inline vector2d<s32> getScreenPos(){return screenPos;}
            inline void addOrder(Order o){orders.push_back(o);}
            inline vector3df getPos() {return pos;}
            inline vector3df* getPosPtr() {return &pos;}
            inline ILightSceneNode* getLight() {return light;}
            inline float getLineOfSight() {return lineOfSight;}
            inline float getWidth() {return width;}
            inline float getHeight() {return height;}
            inline float getLength() {return length;}
            inline ISceneNode* getNode() {return node;}
            inline Player* getPlayer(){return player;}
            inline unitData::UNIT_TYPE getType() {return type;}
            inline void toggleDebugging(bool d){this->debugging=d;}
            inline void takeDamage(int damage) {health -= damage;}
            inline int getId() {return id;}
            inline int getPlayerId() {return playerId;}
            inline vector3df getDirVec() {return dirVec;}
            inline vector3df getLeftVec() {return leftVec;}
            inline vector3df getUpVec() {return upVec;}
        private:
            void updateScreenCoordinates(ICameraSceneNode*, vector3df, vector3df, vector3df);
            void displayUnitStats();
            inline int getNextPatrolPointId() {return patrolPointId == patrolPoints.size() - 1 ? 0 : patrolPointId + 1;}
            inline bool canDisplayOrderLine(){return util::getTime()-orderLineDispTime<orderVecDispLength;}
            ILightSceneNode *light;
            const int orderVecDispLength=2000;
            sf::SoundBuffer *selectionSfxBuffer;
            sf::Sound *selectionSfx=nullptr;
        protected:
            Player *player;
            MoveDir moveDir = MoveDir::FORWARD;
            irr::video::SMaterial createLineMaterial();
            unitData::UNIT_TYPE type;
            ICameraSceneNode *cam = nullptr;
            vector2d<s32> screenPos;
            std::vector<Order> orders;
            std::vector<vector3df> patrolPoints;
            vector3df pos = vector3df(0, 0, 0), upVec = vector3df(0, 1, 0), dirVec = vector3df(0, 0, -1), leftVec = vector3df(1, 0, 0);
            int health, cost, id, patrolPointId = 0, playerId;
            s64 orderLineDispTime=0;
            IAnimatedMesh *mesh;
            ISceneNode *node;
            bool selected = false, selectable, debugging = false, working=true;
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
}

#endif
