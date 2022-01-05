#include <algorithm>
#include <quaternion.h>
#include <model.h>
#include <mesh.h>
#include <material.h>
#include <texture.h>

#include "vessel.h"
#include "util.h"
#include "vesselData.h"
#include "shell.h"
#include "guidedMissile.h"
#include "projectileData.h"

using namespace std;
using namespace vb01;

namespace battleship{
		using namespace unitData;

    Vessel::Turret::Turret(Unit *vessel, int unitId, int turretId) {
        this->vessel = vessel;
        hull = vessel->getNode()->getChild(0);
        this->unitId = unitId;
        this->turretId = turretId;

				string fr[]{DEFAULT_TEXTURE};
        Texture *turretDiffTex = new Texture(fr, 1, false);

        GameManager *gm = GameManager::getSingleton();
				Root *root = Root::getSingleton();
				string libPath = root->getLibPath();
				Material *turretMat = new Material(libPath + "texture");
				turretMat->addBoolUniform("texturingEnabled", true);
				turretMat->addBoolUniform("lightingEnabled", false);
				turretMat->addTexUniform("textures[0]", turretDiffTex, true);

				turret = hull->getChild(turretId);
				turret->getMesh(0)->setMaterial(turretMat);

				for(int i = 0; i < numberOfMantlets[unitId][turretId]; i++){
						Mantlet mantlet;
						mantlet.mantletNode = turret->getChild(i);
						mantlet.mantletNode->getMesh(0)->setMaterial(turretMat);
						mantlet.barrelNode = mantlet.mantletNode->getChild(0);
						mantlet.barrelNode->getMesh(0)->setMaterial(turretMat);
						mantlets.push_back(mantlet);
				}

				/*
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
				*/
        
        sfxBuffer = new sf::SoundBuffer();

        if(sfxBuffer->loadFromFile(getExplosionSfxPath().c_str()))
            sfx = new sf::Sound(*sfxBuffer);
    }

    void Vessel::Turret::rotate(double angle) {
        Quaternion rotQuat = Quaternion(PI / 180 * angle, Vector3(0, 1, 0));
        //turret->setOrientation(vector3df(0, turret->getRotation().Y + angle, 0));
        dirVec = rotQuat * dirVec, leftVec = rotQuat * leftVec;
        this->angle += angle;
    }

    void Vessel::Turret::update() {
				Node *rootNode = Root::getSingleton()->getRootNode();
        Vector3 turretPosition = turret->localToGlobalPosition(Vector3::VEC_ZERO);
        float hullAngle = PI / 180 /* hullNode->getRotation().Y*/;
        float turretAngle = PI / 180 * angle;
				/*
        quaternion rotQuat = rotQuat.fromAngleAxis(hullAngle + turretAngle, vector3df(0, 1, 0));
        quaternion offsetQuat = offsetQuat.fromAngleAxis(PI / 180 * unitData::turretAngleOffset[unitId][turretId], vector3df(0, 1, 0));
        dirVec = rotQuat * (offsetQuat * vector3df(0, 0, -1)), leftVec = rotQuat * (offsetQuat * vector3df(1, 0, 0));
        emitter->setDirection(dirVec);
           if(getTime()-lastShotTime>30&&fxNode->isVisible())
               fxNode->setVisible(false);
               */
    }

    void Vessel::Turret::debug() {
				/*
        float length = turretAxisLength[unitId][turretId]*0 + 1;
        IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
        driver->setTransform(ETS_WORLD, IdentityMatrix);
        driver->setMaterial(mat);
        driver->draw3DLine(turretPosition, turretPosition + dirVec*length, SColor(255, 0, 0, 255));
        driver->draw3DLine(turretPosition, turretPosition + leftVec*length, SColor(255, 255, 0, 0));
        driver->draw3DLine(turretPosition, turretPosition + upVec*length, SColor(255, 0, 255, 0));
				*/
    }

    Vector3* Vessel::Turret::getInitDirs(){
        Vector3 *initDirs = new Vector3[2];

        switch(unitData::initDirs[unitId][turretId]){
            case unitData::FORWARD:
                initDirs[0] = vessel->getDirVec();
                initDirs[1] = vessel->getLeftVec();
                break;
            case unitData::BACKWARD:
                initDirs[0] = -vessel->getDirVec();
                initDirs[1] = -vessel->getLeftVec();
                break;
            case unitData::LEFT:
                initDirs[0] = vessel->getLeftVec();
                initDirs[1] = -vessel->getDirVec();
                break;
            case unitData::RIGHT:
                initDirs[0] = -vessel->getLeftVec();
                initDirs[1] = vessel->getDirVec();
                break;
        }
        return initDirs;
    }
    
    void Vessel::Turret::fire() {
				/*
        quaternion rotQuat = rotQuat.fromAngleAxis(barrelAngle / 180 * PI, leftVec);
        int barId = 0;
        vector3df startPos = vessel->getPos() + vessel->getLeftVec() * turretNode->getPosition().X + vessel->getUpVec() * turretNode->getPosition().Y - vessel->getDirVec() * turretNode->getPosition().Z;
        startPos += rotQuat * (leftVec * turretMantletNodes[barId]->getPosition().X + upVec * turretMantletNodes[barId]->getPosition().Y + dirVec * turretMantletNodes[barId]->getPosition().Z);
        startPos += rotQuat * (leftVec * turretBarrelNodes[barId]->getPosition().X + upVec * turretBarrelNodes[barId]->getPosition().Y + dirVec * turretBarrelNodes[barId]->getPosition().Z);
        startPos += rotQuat * (leftVec * projectileData::pos[unitId][0][turretId].X + upVec * projectileData::pos[unitId][0][turretId].Y + dirVec * projectileData::pos[unitId][0][turretId].Z);
        vessel->addProjectile(new Shell(vessel, startPos, rotQuat*dirVec, rotQuat*leftVec, rotQuat*upVec, unitId, 0, turretId));
           fxNode->setVisible(true);
        if(sfx) sfx->play();
        lastShotTime = getTime();
				*/
    }

    Vessel::Vessel(Player *player, Vector3 pos, int id) : Unit(player, pos, id) {
        if (numberOfTurrets[id] > 0)
            for (int i = 0; i < numberOfTurrets[id]; i++)
                turrets.push_back(new Turret(this, id, i));
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
        Vector3 target = *order.targets[0].pos;
        Unit::attack(order);

        for (Turret *t : turrets) {
						Vector3 *initDirs = t->getInitDirs();
						Node *rootNode = Root::getSingleton()->getRootNode();
      			Vector3 turrPos = t->getNode()->localToGlobalPosition(Vector3::VEC_ZERO);

            bool right = t->getLeftVec().getAngleBetween(target - turrPos) > PI / 2;
            float angle = initDirs[0].getAngleBetween(target - turrPos) / PI * 180;
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
        for (Turret *t : turrets)
            t->debug();
    }
}
