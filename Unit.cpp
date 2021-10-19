#include"Unit.h"
#include"InGameAppState.h"
#include"util.h"
#include"UnitData.h"

using namespace game::content::unitData;
using namespace game::core;
using namespace game::util;
using namespace sf;

namespace game{
    namespace content{
        Unit::Unit(GameManager *gM, Player *player, vector3df pos, int id) {
            this->id = id;
            this->player = player;
            gameManager = gM;
            type = unitData::unitType[id];
            this->health = unitData::health[id];
            this->maxTurnAngle = unitData::maxTurnAngle[id];
            this->range = unitData::range[id];
            this->lineOfSight = unitData::lineOfSight[id];
            width = unitData::unitCornerPoints[id][0].X - unitData::unitCornerPoints[id][1].X;
            height = unitData::unitCornerPoints[id][4].Y - unitData::unitCornerPoints[id][0].Y;
            length = unitData::unitCornerPoints[id][3].Z - unitData::unitCornerPoints[id][0].Z;
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            this->speed = unitData::speed[id];
            mesh = smgr->getMesh(basePath[id] + meshPath[id]);
            node = smgr->addAnimatedMeshSceneNode(mesh);
            placeUnit(pos);
            node->setMaterialFlag(EMF_LIGHTING, false);
            ITriangleSelector *tr = smgr->createTriangleSelector((IAnimatedMeshSceneNode*)node);
            node->setTriangleSelector(tr);
            tr->drop();
            ITexture *diffuseTexture = driver->getTexture(basePath[id] + "diffuseMap.png");
            if (diffuseTexture)
                node->setMaterialTexture(0, diffuseTexture);
            else
                node->setMaterialTexture(0, driver->getTexture(DEFAULT_TEXTURE));
            light = smgr->addLightSceneNode(node, vector3df(0, 2, 0), SColor(255, 255, 255, 255), lineOfSight);
            light->setVisible(false);
            node->setVisible(false);
            selectionSfxBuffer=new SoundBuffer();
            path p=PATH+"Sounds/"+unitData::name[id]+"s/selection.ogg";
            if(selectionSfxBuffer->loadFromFile(p.c_str()))
                selectionSfx=new Sound(*selectionSfxBuffer);
        }

        Unit::~Unit() {
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
//             driver->removeTexture(node->getMaterial(0).getTexture(0));
            node->removeChild(light);
            node->getParent()->removeChild(node);
        }

        void Unit::update() {
            if (orders.size() > 0){
                if(selected&&canDisplayOrderLine()){
                    int r=0,g=0,b=0;
                    switch(orders[0].type){
                        case Order::TYPE::MOVE:
                            g=255;
                            break;
                        case Order::TYPE::ATTACK:
                            r=255;
                            break;
                        case Order::TYPE::PATROL:
                            b=255;
                        case Order::TYPE::LAUNCH:
                            r=255,g=255;
                            break;
                    }
                    gameManager->getDevice()->getVideoDriver()->draw3DLine(pos,*(orders[0].targetPos[0]),SColor(255,r,g,b));
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

        void Unit::updateScreenCoordinates(ICameraSceneNode *cam, vector3df camDir, vector3df camLeft, vector3df camUp) {
            vector3df camPos = cam->getPosition();
            vector3df farLeftUpPoint = cam->getViewFrustum()->getFarLeftUp();
            vector3df farRightUpPoint = cam->getViewFrustum()->getFarRightUp();
            vector3df farLeftDownPoint = cam->getViewFrustum()->getFarLeftDown();
            float camH = camPos.getDistanceFrom(farLeftUpPoint) * cos(getAngleBetween(farLeftUpPoint - camPos, camDir));
            float unitH = camPos.getDistanceFrom(pos) * cos(getAngleBetween(pos - camPos, camDir));
            float ratio = unitH / camH;
            float minRatio = ((cam->getViewFrustum()->getNearLeftUp() - camPos) / (farLeftUpPoint - camPos)).getLength();
            if (minRatio <= ratio && ratio <= 1.) {
                float width = farRightUpPoint.getDistanceFrom(farLeftUpPoint) * ratio;
                float height = farLeftUpPoint.getDistanceFrom(farLeftDownPoint) * ratio;
                vector3df edgePoint = (farLeftUpPoint - camPos) * ratio + camPos;
                float angle = getAngleBetween(pos - edgePoint, -camLeft);
                float x = pos.getDistanceFrom(edgePoint) * cos(angle), y = pos.getDistanceFrom(edgePoint) * cos(PI / 2 - angle);
                if (x <= width && y <= height) {
                    selectable = true;
                    screenPos = vector2d<s32>(gameManager->getWidth() * x / width, gameManager->getHeight() * y / height);
                } else
                    selectable = false;
            } else
                selectable = false;
        }

        void Unit::blowUp(){
            working=false;
        }

        void Unit::updateUnitGUIInfo(ICameraSceneNode* cam, vector3df camDir, vector3df camLeft, vector3df camUp) {
            updateScreenCoordinates(cam, camDir, camLeft, camUp);
            if (selected) {
                displayUnitStats();
                drawCuboid();
            }
        }

        void Unit::displayUnitStats() {
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            SColor white = SColor(255, 255, 255, 255);
            driver->draw2DLine(screenPos + vector2d<s32>(-51, 0), screenPos + vector2d<s32>(50, 0), white);
            driver->draw2DLine(screenPos + vector2d<s32>(-51, 0), screenPos + vector2d<s32>(-51, -20), white);
            driver->draw2DLine(screenPos + vector2d<s32>(-50, -20), screenPos + vector2d<s32>(50, -20), white);
            driver->draw2DLine(screenPos + vector2d<s32>(50, 0), screenPos + vector2d<s32>(50, -20), white);
            driver->draw2DRectangle(SColor(255, 0, 200, 0), rect<s32>(screenPos.X - 50, screenPos.Y - 19, screenPos.X - 50 + ((float)health / unitData::health[id]) * 100, screenPos.Y));

        }

        void Unit::drawCuboid() {
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
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
        }

        void Unit::debug() {
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            driver->setTransform(ETS_WORLD, IdentityMatrix);
            driver->setMaterial(createLineMaterial());
            float length = unitAxisLength[id];
            driver->draw3DLine(pos, pos + dirVec * length, SColor(255, 0, 0, 255));
            driver->draw3DLine(pos, pos + leftVec * length, SColor(255, 255, 0, 0));
            driver->draw3DLine(pos, pos + upVec * length, SColor(255, 0, 255, 0));
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
                    {
                        patrol(order);
                    }
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
            orderLineDispTime=getTime();
        }

        void Unit::attack(Order order) {
            vector3df target=*order.targetPos[0];
            if (pos.getDistanceFrom(target) >= range)
                move(order, range);
//             InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE));
//             for(Player *p : inGameState->getPlayers())
//                 for(Unit *u : p->getUnits())
//                     if(u->getPosPtr()==order.targetPos[0]){}
        }

        void Unit::move(Order order, float destOffset) {
            float movementAmmount, radius = getCircleRadius(), angle = 0.;
            vector3df dest = *order.targetPos[0];
            dest.Y = pos.Y;
            vector3df center;
            if (getAngleBetween(leftVec, dest - pos) < PI / 2) {
                moveDir = MoveDir::LEFT;
                center = pos + leftVec*radius;
            } else if (getAngleBetween(-leftVec, dest - pos) < PI / 2) {
                moveDir = MoveDir::RIGHT;
                center = pos - leftVec*radius;
            }
            bool withinCircle = center.getDistanceFrom(dest) < radius ? true : false;
            if (moveDir != MoveDir::FORWARD) {
                if (withinCircle)
                    angle = getAngleBetween(dirVec, center - dest) / PI * 180;
                else
                    angle = getAngleBetween(dirVec, dest - pos) / PI * 180;
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
                    float inDest = ((center - dest).normalize() * radius - (center - dest)).getLength();
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
            pos += dirVec*speed;
            node->setPosition(pos);
        }

        void Unit::turn(float angle) {// rad
            quaternion rotQuat = rotQuat.fromAngleAxis(PI / 180 * angle, vector3df(0, 1, 0));
            node->setRotation(vector3df(node->getRotation().X, node->getRotation().Y + angle, 0));
            dirVec = rotQuat*dirVec, leftVec = rotQuat*leftVec;
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

        void Unit::placeUnit(vector3df p) {
            node->setPosition(p);
            pos = p;
        }

        void Unit::orientUnit(vector3df orientVec){
            vector3df orientVecProj=vector3df(orientVec.X,0,orientVec.Z).normalize();
            float projAngle=getAngleBetween(orientVec,orientVecProj);
            float rotAngle=getAngleBetween(vector3df(0,0,-1),orientVecProj);
            rotAngle*=(orientVec.X<0?1:-1)/PI*180;
            projAngle*=(orientVec.Y>0?1:-1)/PI*180;
            dirVec=orientVecProj;
            upVec=vector3df(0,1,0);
            leftVec=quaternion(0,0,0,1).fromAngleAxis(-PI/2,upVec)*dirVec;
            quaternion rotQuat=quaternion(0,0,0,1).fromAngleAxis(projAngle/180*PI,leftVec);
            dirVec=rotQuat*dirVec;
            upVec=rotQuat*upVec;
            node->setRotation(vector3df(projAngle,rotAngle,0));
        }
        
        vector3df Unit::getCorner(int i) {
            vector3df corner=unitCornerPoints[id][i];
            return pos+(leftVec*corner.X+upVec*corner.Y-dirVec*corner.Z);
        }

        std::vector<Projectile*> Unit::getProjectiles(){
            std::vector<Projectile*> projectiles;
            InGameAppState *inGameState=((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE));
            for(Projectile *p: inGameState->getProjectiles())
                if(p->getUnit()==this)
                    projectiles.push_back(p);
            return projectiles;
        }
        
        void Unit::removeOrder(int id) {
            for (vector3df *v : orders[id].targetPos) {
                bool isUnitPos = false;
                InGameAppState *state = (InGameAppState*) gameManager->getAppState(AppStateTypes::IN_GAME_STATE);
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
            if(selection&&selectionSfx)
                selectionSfx->play();
            orderLineDispTime=getTime();
        }

        SMaterial Unit::createLineMaterial() {
            SMaterial mat;
            mat.Lighting = false;
            return mat;
        }

        void Unit::addProjectile(Projectile *p) {((InGameAppState*)gameManager->getAppState(AppStateTypes::IN_GAME_STATE))->addProjectile(p);}
    }
}
