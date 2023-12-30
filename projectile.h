#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Audio.hpp>

#include <vector.h>
#include <quaternion.h>

#include "gameManager.h"
#include "gameObject.h"
#include "util.h"

namespace vb01{
	class Node;
	class Model;
	class Mesh;
}

namespace battleship {
    class Unit;
    
    class Projectile : public GameObject{
    public:
        Projectile(Unit*, vb01::Node*, vb01::Vector3, vb01::Vector3, vb01::Vector3, vb01::Vector3, int, int, int);
        virtual ~Projectile();
        virtual void update();
        virtual void debug();
        void orientProjectile(vb01::Quaternion);
        inline bool isExploded() {return exploded;}
        inline Unit* getUnit(){return unit;}
	private:
		void initProperties(int);
		void initModel(vb01::Node*);
		void initSound();
    protected:
        virtual void checkForCollision();
        void explode(vb01::Node*);

        vb01::Model *node = nullptr;
        bool exploded = false;
        float speed, rayLength, scale;
        int damage, id, weaponTypeId;
        vb01::Vector3 initPos;
        s64 lastUpdateTime = 0;
        Unit *unit = nullptr;
        sf::SoundBuffer *shotSfxBuffer, *explosionSfxBuffer;
        sf::Sound *shotSfx = nullptr, *explosionSfx = nullptr;
    };
}

#endif
