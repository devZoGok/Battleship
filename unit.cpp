#include <model.h>
#include <quad.h>
#include <material.h>
#include <texture.h>

#include <stateManager.h>

#include <glm.hpp>
#include <ext.hpp>

#include "unit.h"
#include "util.h"
#include "inGameAppState.h"
#include "unitData.h"

using namespace glm;
using namespace vb01;
using namespace std;

namespace battleship{
		using namespace unitData;

    Unit::Unit(Player *player, vb01::Vector3 pos, int id) {
        this->id = id;
        this->player = player;
        this->health = unitData::health[id];
        this->maxTurnAngle = unitData::maxTurnAngle[id];
        this->range = unitData::range[id];
        this->lineOfSight = unitData::lineOfSight[id];
        this->speed = unitData::speed[id];
        type = unitData::unitType[id];
        width = unitData::unitCornerPoints[id][0].x - unitData::unitCornerPoints[id][1].x;
        height = unitData::unitCornerPoints[id][4].y - unitData::unitCornerPoints[id][0].y;
        length = unitData::unitCornerPoints[id][3].z - unitData::unitCornerPoints[id][0].z;

				GameManager *gm = GameManager::getSingleton();
				model = new Model(basePath[id] + meshPath[id]);
				Root *root = Root::getSingleton();
				string libPath = root->getLibPath();

				Material *mat = new Material(libPath + "texture");
				string f[]{DEFAULT_TEXTURE};
        Texture *diffuseTexture = new Texture(f, 1, false);
				mat->addBoolUniform("texturingEnabled", true);
				mat->addBoolUniform("lightingEnabled", false);
				mat->addTexUniform("textures[0]", diffuseTexture, true);

				model->setMaterial(mat);
				root->getRootNode()->attachChild(model);
        placeUnit(pos);

				/*
        light = smgr->addLightSceneNode(node, vector3df(0, 2, 0), SColor(255, 255, 255, 255), lineOfSight);
        light->setVisible(false);
        model->setVisible(false);
				*/

        selectionSfxBuffer = new sf::SoundBuffer();
        string p = PATH + "Sounds/" + unitData::name[id] + "s/selection.ogg";

        if(selectionSfxBuffer->loadFromFile(p.c_str()))
            selectionSfx = new sf::Sound(*selectionSfxBuffer);

				Node *guiNode = root->getGuiNode();

				hpBackground = new Quad(Vector3(lenHpBar, 10, 0), false);
				Material *hpBackgroundMat = new Material(libPath + "gui");
				hpBackgroundMat->addBoolUniform("texturingEnabled", false);
				hpBackgroundMat->addVec4Uniform("diffuseColor", Vector4(0, 0, 0, 1));
				hpBackground->setMaterial(hpBackgroundMat);
				hpBackgroundNode = new Node();
				hpBackgroundNode->attachMesh(hpBackground);
				guiNode->attachChild(hpBackgroundNode);

				hpForeground = new Quad(Vector3(lenHpBar, 10, 0), false);
				Material *hpForegroundMat = new Material(libPath + "gui");
				hpForegroundMat->addBoolUniform("texturingEnabled", false);
				hpForegroundMat->addVec4Uniform("diffuseColor", Vector4(0, 1, 0, 1));
				hpForeground->setMaterial(hpForegroundMat);
				hpForegroundNode = new Node();
				hpForegroundNode->attachMesh(hpForeground);
				guiNode->attachChild(hpForegroundNode);
    }

    Unit::~Unit() {
				/*
				GameManager *gm = GameManager::getSingleton();
        driver->removeTexture(node->getMaterial(0).getTexture(0));
        node->removeChild(light);
        node->getParent()->removeChild(node);
				*/
    }

    void Unit::update() {
        if (orders.size() > 0){
            if(selected && canDisplayOrderLine()){
                int r = 0, g = 0, b = 0;

                switch(orders[0].type){
                    case Order::TYPE::MOVE:
                        g = 255;
                        break;
                    case Order::TYPE::ATTACK:
                        r = 255;
                        break;
                    case Order::TYPE::PATROL:
                        b = 255;
                    case Order::TYPE::LAUNCH:
                        r = 255, g = 255;
                        break;
                }

								//GameManager::getSingleton()->getDevice()->getVideoDriver()->draw3DLine(pos,*(orders[0].targetPos[0]),SColor(255,r,g,b));
            }

            while (patrolPoints.size() > 0 && orders[0].type != Order::TYPE::PATROL) {
                patrolPoints.pop_back();
                patrolPointId = 0;
            }
        }

        executeOrders();

        if (health <= 0) 
            blowUp();
    }

    void Unit::updateScreenCoordinates() {
			Vector3 rotAxis = rot.getAxis();

			if(rotAxis == Vector3::VEC_ZERO)
				rotAxis = Vector3::VEC_I;

			mat4 model = mat4(1.);
			model = translate(model, vec3(pos.x, pos.y, pos.z));
			model = rotate(model, rot.getAngle(), vec3(rotAxis.x, rotAxis.y, rotAxis.z));
			
			Root *root = Root::getSingleton();
			Camera *cam = root->getCamera();
			Vector3 dir = cam->getDirection(), up = cam->getUp();
			Vector3 camPos = cam->getPosition();
			mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + dir.x, camPos.y + dir.y, camPos.z + dir.z), vec3(up.x, up.y, up.z));
			
			float fov = cam->getFov(), width = root->getWidth(), height = root->getHeight(), nearPlane = cam->getNearPlane(), farPlane = cam->getFarPlane();
			mat4 proj = perspective(radians(fov), width / height, nearPlane, farPlane);

			vec4 ndcPos = proj * view * model * vec4(0, 0, 0, 1);
			ndcPos.x /= ndcPos.w;
			ndcPos.y /= ndcPos.w;
			screenPos = Vector2(0.5 * width * (1 + ndcPos.x), 0.5 * height * (1 - ndcPos.y));
    }

    void Unit::blowUp(){
        working = false;
    }

    void Unit::updateUnitGUIInfo() {
        updateScreenCoordinates();

        if (selected) {
            displayUnitStats();
            drawCuboid();
        }
    }

    void Unit::displayUnitStats() {
				float shiftedX = screenPos.x - 0.5 * lenHpBar;
				hpForegroundNode->setPosition(Vector3(shiftedX, screenPos.y, 0));
				hpForeground->setSize(Vector3(health / unitData::health[id] * lenHpBar, 10, 0));
				hpBackgroundNode->setPosition(Vector3(shiftedX, screenPos.y, .1));
    }

    void Unit::drawCuboid() {
				/*
        IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
        driver->setTransform(ETS_WORLD, IdentityMatrix);

        for (int i = 0; i < 8; i++) {
            vector3df vec = unitCornerPoints[id][i];
            vector3df yVec;
            vec = leftVec * vec.X + upVec * vec.Y - dirVec * vec.Z;
            if (0 <= i && i < 4)
                yVec = vector3df(0, unitCuboidDimensions[id][i].Y, 0);
            else if (i >= 4)
                yVec = vector3df(0, -unitCuboidDimensions[id][i].Y, 0);
            quaternion rotQuat = rotQuat.fromAngleAxis(PI / 2 * i, vector3df(0, 1, 0));
            vector3df vec1 = rotQuat * (-leftVec * unitCuboidDimensions[id][i].X), vec2 = rotQuat * (-dirVec * unitCuboidDimensions[id][i].Z);
            driver->draw3DLine(pos + vec, pos + vec + vec1);
            driver->draw3DLine(pos + vec, pos + vec + vec2);
            driver->draw3DLine(pos + vec, pos + vec + yVec);
        }
				*/
    }

    void Unit::debug() {
		/*
        IVideoDriver *driver = GameManager::getSingleton()->getDevice()->getVideoDriver();
        driver->setTransform(ETS_WORLD, IdentityMatrix);
        driver->setMaterial(createLineMaterial());
        float length = unitAxisLength[id];
        driver->draw3DLine(pos, pos + dirVec * length, SColor(255, 0, 0, 255));
        driver->draw3DLine(pos, pos + leftVec * length, SColor(255, 255, 0, 0));
        driver->draw3DLine(pos, pos + upVec * length, SColor(255, 0, 255, 0));
		*/
    }

    void Unit::executeOrders() {
        if (orders.size() > 0) {
            Order order = orders[0];

            switch (order.type) {
                case Order::TYPE::ATTACK:
                    attack(order);
                    break;
                case Order::TYPE::MOVE:
                    move(order, unitData::destinationOffset[id]);
                    break;
                case Order::TYPE::PATROL:
                    patrol(order);
                    break;
                case Order::TYPE::LAUNCH:
                    launch(order);
                    break;
                default:
                    break;
            }
        }
    }

    void Unit::setOrder(Order order) {
        while (orders.size() > 0)
            orders.pop_back();

        addOrder(order);
        orderLineDispTime = getTime();
    }

    void Unit::attack(Order order) {
        Vector3 target = *order.targetPos[0];

        if (pos.getDistanceFrom(target) >= range)
            move(order, range);
           InGameAppState *inGameState=((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

           for(Player *p : inGameState->getPlayers())
               for(Unit *u : p->getUnits())
                   if(u->getPosPtr()==order.targetPos[0]){}
    }

    void Unit::move(Order order, float destOffset) {
        float movementAmmount, radius = getCircleRadius(), angle = 0.;
        Vector3 dest = *order.targetPos[0];
        dest.y = pos.y;
        Vector3 center;

        if (leftVec.getAngleBetween(dest - pos) < PI / 2) {
            moveDir = MoveDir::LEFT;
            center = pos + leftVec*radius;
        } else if (-leftVec.getAngleBetween(dest - pos) < PI / 2) {
            moveDir = MoveDir::RIGHT;
            center = pos - leftVec*radius;
        }

        bool withinCircle = center.getDistanceFrom(dest) < radius ? true : false;

        if (moveDir != MoveDir::FORWARD) {
            if (withinCircle)
                angle = dirVec.getAngleBetween(center - dest) / PI * 180;
            else
                angle = dirVec.getAngleBetween(dest - pos) / PI * 180;

            angle = isnan(angle) ? 0. : angle;

            if (angle > anglePrecision[id]) {
                float rotAngle = maxTurnAngle > angle ? angle : maxTurnAngle;
                angle -= rotAngle;
                turn(moveDir == MoveDir::RIGHT ? rotAngle : -rotAngle);
            }
        }

        if (angle > anglePrecision[id])
            movementAmmount = speed;
        else {
            if (withinCircle) {
                float inDest = ((center - dest).norm() * radius - (center - dest)).getLength();
                movementAmmount = speed > inDest ? inDest : speed;
            } else
                movementAmmount = speed > pos.getDistanceFrom(dest) ? pos.getDistanceFrom(dest) : speed;
        }

        advance(movementAmmount);

        if (pos.getDistanceFrom(dest) <= destOffset && orders[0].type == Order::TYPE::MOVE) {
            moveDir = MoveDir::FORWARD;
            removeOrder(0);
        }
    }

    void Unit::patrol(Order order) {
        move(order, unitData::destinationOffset[id]);
    }

    void Unit::launch(Order order) {}

    void Unit::advance(float speed) {
        pos = pos + dirVec * speed;
        model->setPosition(pos);
    }

    void Unit::turn(float angle) {// rad
				/*
        Quaternion rotQuat = Quaternion(PI / 180 * angle, Vector3(0, 1, 0));
        node->setOrientation(Vector3(node->getOrientation().x, node->getRotation().Y + angle, 0));
        dirVec = rotQuat*dirVec, leftVec = rotQuat*leftVec;
				*/
    }

    void Unit::halt() {
        while (orders.size() > 0)
            removeOrder(orders.size() - 1);
    }

    float Unit::getCircleRadius() {
        float radius = 0.;

        for (int i = 0; i <= 90 / maxTurnAngle; i++)
            radius += cos(i * (maxTurnAngle / 180 * PI)) * speed;

        return radius;
    }

    void Unit::placeUnit(Vector3 p) {
        model->setPosition(p);
        pos = p;
    }

    void Unit::orientUnit(Vector3 orientVec){
        Vector3 orientVecProj = Vector3(orientVec.x, 0, orientVec.z).norm();
        float projAngle = orientVec.getAngleBetween(orientVecProj);
        float rotAngle = Vector3(0,0,-1).getAngleBetween(orientVecProj);

        rotAngle *= (orientVec.x < 0 ? 1 : -1) / PI * 180;
        projAngle *= (orientVec.y > 0 ? 1 : -1) / PI * 180;
        dirVec = orientVecProj;
        upVec = Vector3(0,1,0);
        leftVec = Quaternion(-PI / 2, upVec) * dirVec;
        Quaternion rotQuat = Quaternion(projAngle / 180 * PI, leftVec);
        dirVec = rotQuat * dirVec;
        upVec = rotQuat * upVec;
        //node->setRotation(vector3df(projAngle,rotAngle,0));
    }
    
    Vector3 Unit::getCorner(int i) {
        Vector3 corner = unitCornerPoints[id][i];
        return pos + (leftVec * corner.x + upVec * corner.y - dirVec * corner.z);
    }

    std::vector<Projectile*> Unit::getProjectiles(){
        std::vector<Projectile*> projectiles;
        InGameAppState *inGameState = ((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

        for(Projectile *p: inGameState->getProjectiles())
            if(p->getUnit() == this)
                projectiles.push_back(p);

        return projectiles;
    }
    
    void Unit::removeOrder(int id) {
        for (Vector3 *v : orders[id].targetPos) {
            bool isUnitPos = false;
            InGameAppState *state = (InGameAppState*) GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE);

            for (Player *p : state->getPlayers())
                for (Unit *u : p->getUnits())
                    if (v == u->getPosPtr())
                        isUnitPos = true;

            if(!isUnitPos)    
                delete v;
        }

        orders.erase(orders.begin() + id);
    }

    void Unit::toggleSelection(bool selection) {
        selected = selection;

        if(selection && selectionSfx)
            selectionSfx->play();

        orderLineDispTime = getTime();
    }

		/*
    SMaterial Unit::createLineMaterial() {
        SMaterial mat;
        mat.Lighting = false;
        return mat;
    }
		*/

    void Unit::addProjectile(Projectile *p) {((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE))->addProjectile(p);}
}
