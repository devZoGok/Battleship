#include"Vessel.h"
#include"util.h"
#include"VesselData.h"
#include"Shell.h"
#include"GuidedMissile.h"
#include"ProjectileData.h"
#include<algorithm>

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::io;
using namespace irr::video;
using namespace irr::scene;
using namespace sf;

namespace game{
    namespace content{
        Vessel::Turret::Turret(GameManager *gM, Unit *vessel, int unitId, int turretId) {
            gameManager = gM;
            this->vessel = vessel;
            hullNode = vessel->getNode();
            this->unitId = unitId;
            this->turretId = turretId;
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            turretMesh = smgr->getMesh(basePath[unitId] + turretNames[unitId][turretId] + ".x");
            turretNode = smgr->addAnimatedMeshSceneNode(turretMesh);
            turretNode->setMaterialFlag(EMF_LIGHTING, false);
            ITexture *turretDiffuseTexture = driver->getTexture(basePath[unitId] + turretNames[unitId][turretId] + "DiffuseMap.png");
            if (turretDiffuseTexture)
                turretNode->setMaterialTexture(0, turretDiffuseTexture);
            else
                turretNode->setMaterialTexture(0, driver->getTexture(DEFAULT_TEXTURE));
            turretNode->setParent(vessel->getNode());
            turretNode->setPosition(turretPos[unitId][turretId]);
            quaternion rotQuat = rotQuat.fromAngleAxis(PI / 180 * turretAngleOffset[unitId][turretId], vector3df(0, 1, 0));
            turretNode->setRotation(vector3df(0, turretAngleOffset[unitId][turretId], 0));
            dirVec = rotQuat*dirVec, leftVec = rotQuat*leftVec;
            rotationSpeed = unitData::turretRotationSpeed[unitId][turretId];
            maxAngle = unitData::turretMaxAngle[unitId][turretId];
            this->rateOfFire = unitData::turretRateOfFire[unitId][turretId];
            
            for (int i = 0; i < numberOfMantlets[unitId][turretId]; i++) {
                IAnimatedMesh *mantletMesh = smgr->getMesh(basePath[unitId] + mantletNames[unitId][turretId] + ".x");
                IAnimatedMesh *barrelMesh = smgr->getMesh(basePath[unitId] + barrelNames[unitId][turretId] + ".x");
                ISceneNode *mantletNode = initMantletNode(driver, smgr, mantletMesh, unitId, turretId);
                ISceneNode *barrelNode = initBarrelNode(driver, smgr, barrelMesh, mantletNode, unitId, turretId);
                turretMantletMeshes.push_back(mantletMesh);
                turretMantletNodes.push_back(mantletNode);
                turretBarrelMeshes.push_back(barrelMesh);
                turretBarrelNodes.push_back(barrelNode);
            }
            if (turretMantletNodes.size() > 1) {
                turretMantletNodes[0]->setPosition(turretMantletNodes[0]->getPosition() + vector3df(mantletPosSideOffset[unitId][turretId], 0, 0));
                turretMantletNodes[1]->setPosition(turretMantletNodes[1]->getPosition() - vector3df(mantletPosSideOffset[unitId][turretId], 0, 0));
            }

            fxNode=smgr->addParticleSystemSceneNode(false);
            emitter=fxNode->createPointEmitter(dirVec,
                                               10,10,
                                               SColor(0,255,255,255),
                                               SColor(0,255,255,255),
                                               500,500,0,
                                               dimension2df(5,5),
                                               dimension2df(10,10));
            fxNode->setEmitter(emitter);
            emitter->drop();
            affector=fxNode->createFadeOutParticleAffector();
            fxNode->addAffector(affector);
            affector->drop();
            fxNode->setParent(turretMantletNodes[0]);
            fxNode->setMaterialFlag(EMF_LIGHTING, false);
            fxNode->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
            fxNode->setMaterialTexture(0, driver->getTexture(PATH+"Textures/Explosions/fire.bmp"));
            fxNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
            fxNode->setVisible(false);
            
            sfxBuffer=new SoundBuffer();
            if(sfxBuffer->loadFromFile(getExplosionSfxPath().c_str()))
                sfx=new Sound(*sfxBuffer);
        }

        ISceneNode* Vessel::Turret::initMantletNode(IVideoDriver *driver, ISceneManager *smgr, IAnimatedMesh *mantletMesh, int unitId, int turretId) {
            ISceneNode *mantletNode = smgr->addAnimatedMeshSceneNode(mantletMesh);
            mantletNode->setMaterialFlag(EMF_LIGHTING, false);
            ITexture *mantletDiffuseTexture = driver->getTexture(basePath[unitId] + mantletNames[unitId][turretId] + "DiffuseMap.png");
            if (mantletDiffuseTexture)
                mantletNode->setMaterialTexture(0, mantletDiffuseTexture);
            else
                mantletNode->setMaterialTexture(0, driver->getTexture(DEFAULT_TEXTURE));
            mantletNode->setParent(turretNode);
            mantletNode->setPosition(mantletPos[unitId][turretId]);
            return mantletNode;
        }

        ISceneNode* Vessel::Turret::initBarrelNode(IVideoDriver *driver, ISceneManager *smgr, IAnimatedMesh *barrelMesh, ISceneNode *mantletNode, int unitId, int turretId) {
            ISceneNode *barrelNode = smgr->addAnimatedMeshSceneNode(barrelMesh);
            barrelNode->setMaterialFlag(EMF_LIGHTING, false);
            ITexture *barrelDiffuseTexture = driver->getTexture(basePath[unitId] + barrelNames[unitId][turretId] + "DiffuseMap.png");
            if (barrelDiffuseTexture)
                barrelNode->setMaterialTexture(0, barrelDiffuseTexture);
            else
                barrelNode->setMaterialTexture(0, driver->getTexture(DEFAULT_TEXTURE));
            barrelNode->setParent(mantletNode);
            barrelNode->setPosition(barrelPos[unitId][turretId]);
            return barrelNode;
        }

        void Vessel::Turret::rotate(double angle) {
            quaternion rotQuat = rotQuat.fromAngleAxis(PI / 180 * angle, vector3df(0, 1, 0));
            turretNode->setRotation(vector3df(0, turretNode->getRotation().Y + angle, 0));
            dirVec = rotQuat*dirVec, leftVec = rotQuat*leftVec;
            this->angle += angle;
        }

        void Vessel::Turret::update() {
            turretPosition = turretNode->getAbsolutePosition();
            float hullAngle = PI / 180 * hullNode->getRotation().Y;
            float turretAngle = PI / 180 * angle;
            quaternion rotQuat = rotQuat.fromAngleAxis(hullAngle + turretAngle, vector3df(0, 1, 0));
            quaternion offsetQuat = offsetQuat.fromAngleAxis(PI / 180 * unitData::turretAngleOffset[unitId][turretId], vector3df(0, 1, 0));
            dirVec = rotQuat * (offsetQuat * vector3df(0, 0, -1)), leftVec = rotQuat * (offsetQuat * vector3df(1, 0, 0));
            emitter->setDirection(dirVec);
//             if(getTime()-lastShotTime>30&&fxNode->isVisible())
//                 fxNode->setVisible(false);
        }

        void Vessel::Turret::debug(const SMaterial &mat) {
            float length = turretAxisLength[unitId][turretId]*0 + 1;
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            driver->setTransform(ETS_WORLD, IdentityMatrix);
            driver->setMaterial(mat);
            driver->draw3DLine(turretPosition, turretPosition + dirVec*length, SColor(255, 0, 0, 255));
            driver->draw3DLine(turretPosition, turretPosition + leftVec*length, SColor(255, 255, 0, 0));
            driver->draw3DLine(turretPosition, turretPosition + upVec*length, SColor(255, 0, 255, 0));
        }

        vector3df* Vessel::Turret::getInitDirs(){
            vector3df *initDirs=new vector3df[2];
            switch(unitData::initDirs[unitId][turretId]){
                case unitData::FORWARD:
                    initDirs[0]=vessel->getDirVec();
                    initDirs[1]=vessel->getLeftVec();
                    break;
                case unitData::BACKWARD:
                    initDirs[0]=-vessel->getDirVec();
                    initDirs[1]=-vessel->getLeftVec();
                    break;
                case unitData::LEFT:
                    initDirs[0]=vessel->getLeftVec();
                    initDirs[1]=-vessel->getDirVec();
                    break;
                case unitData::RIGHT:
                    initDirs[0]=-vessel->getLeftVec();
                    initDirs[1]=vessel->getDirVec();
                    break;
            }
            return initDirs;
        }
        
        void Vessel::Turret::fire() {
            quaternion rotQuat = rotQuat.fromAngleAxis(barrelAngle / 180 * PI, leftVec);
            int barId = 0;
            vector3df startPos = vessel->getPos() + vessel->getLeftVec() * turretNode->getPosition().X + vessel->getUpVec() * turretNode->getPosition().Y - vessel->getDirVec() * turretNode->getPosition().Z;
            startPos += rotQuat * (leftVec * turretMantletNodes[barId]->getPosition().X + upVec * turretMantletNodes[barId]->getPosition().Y + dirVec * turretMantletNodes[barId]->getPosition().Z);
            startPos += rotQuat * (leftVec * turretBarrelNodes[barId]->getPosition().X + upVec * turretBarrelNodes[barId]->getPosition().Y + dirVec * turretBarrelNodes[barId]->getPosition().Z);
            startPos += rotQuat * (leftVec * projectileData::pos[unitId][0][turretId].X + upVec * projectileData::pos[unitId][0][turretId].Y + dirVec * projectileData::pos[unitId][0][turretId].Z);
            vessel->addProjectile(new Shell(gameManager, vessel, startPos, rotQuat*dirVec, rotQuat*leftVec, rotQuat*upVec, unitId, 0, turretId));
//             fxNode->setVisible(true);
            if(sfx) sfx->play();
            lastShotTime = getTime();
        }

        Vessel::Vessel(GameManager *gM, Player *player,vector3df pos, int id) : Unit(gM, player,pos, id) {
            if (numberOfTurrets[id] > 0)
                for (int i = 0; i < numberOfTurrets[id]; i++)
                    turrets.push_back(new Turret(Unit::gameManager, this, id, i));
        }

        void Vessel::update() {
            Unit::update();
            for (Turret *t : turrets) {
                t->update();
                if ((orders.size() > 0 && orders[0].type != Order::TYPE::ATTACK) || orders.size() == 0) {
                    float rotAngle = abs(t->getAngle()) < t->getRotationSpeed() ? abs(t->getAngle()) : t->getRotationSpeed();
                    t->rotate(t->getAngle() > 0 ? -rotAngle : rotAngle);
                }
            }
        }

        void Vessel::attack(Order order) {
            vector3df target=*order.targetPos[0];
            Unit::attack(order);
            for (Turret *t : turrets) {
                vector3df *initDirs=t->getInitDirs();
                bool right = getAngleBetween(t->getLeftVec(), target - t->getPos()) > PI / 2;
                float angle = getAngleBetween(initDirs[0], target - t->getPos()) / PI * 180;
                float rotAngle = t->getRotationSpeed() > angle ? angle : t->getRotationSpeed()*(right?1:-1);
                if (-t->getMaxAngle() <= t->getAngle() + rotAngle 
                    && t->getAngle() + rotAngle <= t->getMaxAngle()) {
                    t->rotate(rotAngle);
                    if (t->canFire()) t->fire();
                }
                delete[] initDirs;
            }
        }

        void Vessel::debug() {
//             Unit::debug();
            for (Turret *t : turrets)
                t->debug(createLineMaterial());
        }
    }
}
