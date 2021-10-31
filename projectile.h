#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Audio.hpp>
#include <vector.h>

#include "gameManager.h"
#include "util.h"

namespace vb01{
		class Node;
		class Model;
		class Mesh;
}

namespace game{
    namespace content {
        class Unit;
        
        class Projectile {
        public:
            Projectile(Unit*, vb01::Node*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
            virtual ~Projectile();
            virtual void update();
            virtual void debug();
            void orientProjectile(vb01::Vector3);
            inline bool isExploded() {return exploded;}
            inline Unit* getUnit(){return unit;}
        protected:
            virtual void checkForCollision();
            void explode(vb01::Node*);

            vb01::Model *node=nullptr;
            bool exploded = false;
            const float g = 2.2;
            float speed, angle, rayLength, scale;
            int damage,id,weaponTypeId;
            vb01::Vector3 initPos, pos, dirVec, leftVec, upVec;
            s64 lastUpdateTime = 0;
            Unit *unit;
            sf::SoundBuffer *shotSfxBuffer, *explosionSfxBuffer;
            sf::Sound *shotSfx=nullptr, *explosionSfx=nullptr;
        };
    }
}

#endif
