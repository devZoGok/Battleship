#pragma once
#ifndef VESSEL_H
#define VESSEL_H

#include "unit.h"
#include "util.h"

namespace game{
    namespace content {
        class Vessel : public Unit {
        public:
            Vessel(core::GameManager*, Player*,vector3df, int);
            void update();
        protected:
        void attack(Order);
        private:
            void debug();
            class Turret {
            public:
                Turret(core::GameManager*, Unit*, int, int);
                void update();
                void fire();
                void rotate(double);
                void debug(const SMaterial&);
                vector3df* getInitDirs();
                inline vector3df getDirVec(){return dirVec;}
                inline vector3df getLeftVec(){return leftVec;}
                inline vector3df getUpVec(){return upVec;}
                inline vector3df getPos(){return turretPosition;}
                inline float getMaxAngle(){return maxAngle;}
                inline float getRotationSpeed(){return rotationSpeed;}
                inline float getAngle(){return angle;}
                inline bool canFire(){return util::getTime()-lastShotTime>rateOfFire;}
            private:
                inline irr::io::path getExplosionSfxPath(){return core::PATH+"Sounds/Explosions/explosion0"+stringw(rand()%4)+".ogg";}
                irr::scene::IParticleSystemSceneNode *fxNode;
                irr::scene::IParticleEmitter *emitter;
                irr::scene::IParticleAffector *affector;
                core::GameManager *gameManager;
                Unit *vessel;
                irr::scene::IAnimatedMesh *turretMesh;
                irr::scene::ISceneNode *turretNode, *hullNode;
                std::vector<irr::scene::IAnimatedMesh*> turretMantletMeshes, turretBarrelMeshes;
                std::vector<irr::scene::ISceneNode*> turretMantletNodes, turretBarrelNodes;
                std::vector<vector3df> turretMantletPos;
                irr::core::vector3df turretBarrelPos, turretPosition;
                irr::core::vector3df upVec = irr::core::vector3df(0, 1, 0),dirVec = irr::core::vector3df(0, 0, -1),initDir,leftVec = irr::core::vector3df(1, 0, 0);
                float angle = 0., barrelAngle = 0., maxAngle, rotationSpeed,maxBarrelAngle;
                int rateOfFire, damage, unitId, turretId;
                ISceneNode* initMantletNode(irr::video::IVideoDriver*, irr::scene::ISceneManager*, irr::scene::IAnimatedMesh*, int, int);
                ISceneNode* initBarrelNode(irr::video::IVideoDriver*, ISceneManager*, irr::scene::IAnimatedMesh*, irr::scene::ISceneNode*, int, int);
                s64 lastShotTime;
                sf::SoundBuffer *sfxBuffer;
                sf::Sound *sfx=nullptr;
            };
            std::vector<Turret*> turrets;
        };
    }
}

#endif
