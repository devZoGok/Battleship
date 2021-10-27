#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <irrlicht.h>
#include <SFML/Audio.hpp>

#include "gameManager.h"
#include "util.h"

namespace game{
    namespace content {
        class Unit;
        
        class Projectile {
        public:
            Projectile(core::GameManager*, Unit*, irr::scene::ISceneNode*, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, irr::core::vector3df, int, int, int);
            virtual ~Projectile();
            virtual void update();
            virtual void debug();
            void orientProjectile(irr::core::vector3df);
            inline bool isExploded() {return exploded;}
            inline Unit* getUnit(){return unit;}
        protected:
            virtual void checkForCollision();
            void explode(irr::scene::ISceneNode*);
            core::GameManager *gameManager;
            irr::scene::IAnimatedMesh *mesh;
            irr::scene::ISceneNode *node=nullptr;
            bool exploded = false;
            const float g = 2.2;
            float speed, angle, rayLength, scale;
            int damage,id,weaponTypeId;
            irr::core::vector3df initPos, pos, dirVec, leftVec, upVec;
            s64 lastUpdateTime = 0;
            Unit *unit;
            sf::SoundBuffer *shotSfxBuffer, *explosionSfxBuffer;
            sf::Sound *shotSfx=nullptr, *explosionSfx=nullptr;
        };
    }
}

#endif
