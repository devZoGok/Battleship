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
				leftVec = model->getGlobalAxis(0);
				upVec = model->getGlobalAxis(1);
				dirVec = model->getGlobalAxis(2);

        if (!orders.empty()){
						LineRenderer *lineRenderer = LineRenderer::getSingleton();
						lineRenderer->toggleVisibility(orders[0].line.id, selected && canDisplayOrderLine());
						lineRenderer->changeLineField(orders[0].line.id, pos, LineRenderer::START);

            while (patrolPoints.size() > 0 && orders[0].type != Order::TYPE::PATROL) {
                patrolPoints.pop_back();
                patrolPointId = 0;
            }
        }

        executeOrders();

				screenPos = spaceToScreen(pos);
				hpBackgroundNode->setVisible(selected);
				hpForegroundNode->setVisible(selected);

        if (selected) {
            displayUnitStats();
            drawCuboid();
        }

        if (health <= 0) 
            blowUp();
    }

    void Unit::blowUp(){
        working = false;
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
        while (!orders.empty())
						removeOrder(0);

        addOrder(order);
        orderLineDispTime = getTime();
    }

    void Unit::attack(Order order) {
    }

    void Unit::move(Order order, float destOffset) {
        float movementAmmount, radius = getCircleRadius(), angle = 0.;
        Vector3 dest = *order.targets[0].pos;
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
                turn(moveDir == MoveDir::RIGHT ? -rotAngle : rotAngle);
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

    void Unit::turn(float angle) {
        Quaternion rotQuat = Quaternion(PI / 180 * angle, Vector3(0, 1, 0));
        model->setOrientation(rotQuat * model->getOrientation());
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
				LineRenderer::getSingleton()->removeLine(orders[id].line.id);

				for(Order::Target t : orders[id].targets)
						if(t.unit)
								delete t.pos;

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
