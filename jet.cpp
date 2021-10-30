#include "jet.h"
#include "jetData.h"
#include "aircraftCarrier.h"

using namespace game::core;
using namespace game::util;
using namespace game::content::unitData;

namespace game{
    namespace content{
        Jet::Jet(Player *player, vector3df pos, int unitId, bool onBoard) : Unit(player, pos, unitId) {
            offsetPos = pos;
            this->onBoard = onBoard;
            this->pitchSpeed = unitData::pitchSpeed[id];
        }

        void Jet::update() {
            Unit::update();
            vector3df lp = aircraftCarrier->getDirVec() * aircraftCarrier->getRunwayLength() + aircraftCarrier->getJetPos(jetId);
            if (!onBoard && aircraftCarrier) {
                if (orders.size() == 0) {
                    Order o;
                    o.type = Order::TYPE::MOVE;
                    o.targetPos.push_back(new vector3df(lp));
                    setOrder(o);
                }
                if (!landing
                        &&aircraftCarrier
                        &&pos.getDistanceFrom(lp) <= unitData::destinationOffset[id]
                        &&*orders[0].targetPos[0] == lp){
                    float angle=getAngleBetween(dirVec,aircraftCarrier->getDirVec()),maxAngle=PI-anglePrecision[id]/180*PI;
                    if(!toTurnPoint&&anglePrecision[id]/180*PI<angle&&angle<maxAngle){
                        float radius=getCircleRadius();
                        vector3df carrierSideVec=aircraftCarrier->getLeftVec().normalize();
                        vector3df jetSideVec=leftVec.normalize();
                        if(getAngleBetween(carrierSideVec,dirVec)<PI/2){
                            carrierSideVec=-carrierSideVec;
                            jetSideVec=-jetSideVec;
                        }
                        vector3df tanPoint=pos+radius*(jetSideVec+carrierSideVec);
                        float ang1=getAngleBetween(tanPoint-lp,carrierSideVec);
                        float ang2=getAngleBetween(dirVec,-carrierSideVec);
                        float base=(tanPoint-lp).getLength()*cos(ang1);
                        float hyp=base/cos(ang2);
                        landingPos=(pos+dirVec*hyp);
                        toTurnPoint=true;
                    }
                    else if(PI-angle<anglePrecision[id]/180*PI){
                        onBoard=true;
                        landing=true;
                        orientUnit(-aircraftCarrier->getDirVec());
                    }
                }
            }
        }

        void Jet::move(Order order, float offset) {
            if(toTurnPoint)
                turnAround();
            else{
                if(!onBoard)
                    Unit::move(order, offset);
                else{
                    if(landing)
                        land();
                    else
                        takeOff();
                }
            }
        }

        void Jet::lap(vector3df destDir) {
            vector3df center = pos - leftVec * getCircleRadius();
            vector3df initCircleDir = -leftVec;
            float angle = getAngleBetween(initCircleDir, destDir);
            angle = quaternion(0, 0, 0, 0).fromAngleAxis(angle, upVec) * initCircleDir == destDir ? angle : 2 * PI - angle;
            vector3df offsetDir = quaternion(0, 0, 0, 0).fromAngleAxis(PI / 2, upVec) * destDir;
            vector3df destDirPos = quaternion(0, 0, 0, 0).fromAngleAxis(angle, upVec) * dirVec;
            vector3df offsetDirPos = quaternion(0, 0, 0, 0).fromAngleAxis(angle + PI / 2, upVec) * dirVec;
            vector3df hypVec = *orders[0].targetPos[0] - destDirPos;
            float triAngle = getAngleBetween(hypVec, offsetDir);
            float distance = cos(triAngle) * hypVec.getLength();
            vector3df destPoint = offsetDirPos + offsetDirPos.normalize() * distance;
            float dirAngle=getAngleBetween(dirVec,offsetDir)*180/PI;
            float movementAmount;
            turn(maxTurnAngle>dirAngle?dirAngle:maxTurnAngle);
            if(dirAngle>unitData::anglePrecision[id])
                movementAmount=speed;
            else{
                float destDistance=(destPoint-pos).getLength();
                movementAmount=speed>destDistance?destDistance:speed;
            }
            advance(movementAmount);
        }

        void Jet::takeOff() {
            float distance = aircraftCarrier->getRunwayLength()-(pos - aircraftCarrier->getJetPos(jetId)).getLength();
            float movementAmmount = speed > distance ? distance : speed;
            advance(movementAmmount);
            distance -= movementAmmount;
            if (distance <= 0.)
                onBoard = false;
        }

        void Jet::land(){
            float distance = (pos - aircraftCarrier->getJetPos(jetId)).getLength();
            float movementAmmount = speed > distance ? distance : speed;
            advance(movementAmmount);
            distance -= movementAmmount;
            if (distance <= 0.){
                landing = false;
                while(!orders.empty())
                    orders.pop_back();
            }
        }
        
        void Jet::turnAround() {
            float distance=pos.getDistanceFrom(landingPos);
            float movementAmmount = speed > distance ? distance : speed;
            advance(movementAmmount);
            distance -= movementAmmount;
            if (distance <= 0.)
                toTurnPoint = false;
        }

        void Jet::pitch(float angle) {
            quaternion rotQuat = rotQuat.fromAngleAxis(-angle / 180 * PI, leftVec);
            dirVec = rotQuat*dirVec, upVec = rotQuat*upVec;
            horAngle = angle;
            float yAngle = node->getRotation().Y;
            node->setRotation(vector3df(angle, 0, 0));
            node->setRotation(vector3df(angle, yAngle, 0));
        }
    }
}
