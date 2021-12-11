#pragma once
#ifndef VESSEL_H
#define VESSEL_H

#include <vector.h>
#include <util.h>

#include "unit.h"

namespace vb01{
		class Model;
		class Material;
		class ParticleEmitter;
}

namespace battleship {
    class Vessel : public Unit {
    public:
        Vessel(Player*, vb01::Vector3, int);
        void update();
    protected:
    	void attack(Order);
    private:
        void debug();

        class Turret {
        public:
						struct Mantlet{
								vb01::Node *mantletNode = nullptr, *barrelNode = nullptr;
						};

            Turret(Unit*, int, int);
            void update();
            void fire();
            void rotate(double);
            void debug();
						vb01::Vector3* getInitDirs();
            inline vb01::Vector3 getDirVec(){return dirVec;}
            inline vb01::Vector3 getLeftVec(){return leftVec;}
            inline vb01::Vector3 getUpVec(){return upVec;}
						inline vb01::Node* getNode(){return turret;}
            inline float getMaxAngle(){return maxAngle;}
            inline float getRotationSpeed(){return rotationSpeed;}
            inline float getAngle(){return angle;}
            inline bool canFire(){return vb01::getTime() - lastShotTime > rateOfFire;}
        private:
            inline std::string getExplosionSfxPath(){return configData::PATH + "Sounds/Explosions/explosion0" + std::to_string(rand() % 4) + ".ogg";}

            vb01::Node *fxNode;
            vb01::ParticleEmitter *emitter;
            Unit *vessel;
            vb01::Node *turret = nullptr, *hull = nullptr;
            std::vector<Mantlet> mantlets;
            vb01::Vector3 upVec = vb01::Vector3(0, 1, 0), dirVec = vb01::Vector3(0, 0, -1), initDir, leftVec = vb01::Vector3(1, 0, 0);
            float angle = 0., barrelAngle = 0., maxAngle, rotationSpeed,maxBarrelAngle;
            int rateOfFire, damage, unitId, turretId;
            s64 lastShotTime;
            sf::SoundBuffer *sfxBuffer;
            sf::Sound *sfx = nullptr;
        };

        std::vector<Turret*> turrets;
    };
}

#endif
