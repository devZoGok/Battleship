#include <quaternion.h>
#include <model.h>

#include "jet.h"
#include "jetData.h"
#include "aircraftCarrier.h"

using namespace vb01;

namespace battleship{
	using namespace unitData;

    Jet::Jet(Player *player, Vector3 pos, int unitId, bool onBoard) : Unit(player, pos, unitId) {
        offsetPos = pos;
        this->onBoard = onBoard;
        this->pitchSpeed = unitData::pitchSpeed[id];
    }

    void Jet::update() {
        Unit::update();
        Vector3 lp = aircraftCarrier->getDirVec() * aircraftCarrier->getRunwayLength() + aircraftCarrier->getJetPos(jetId);

        if (!onBoard && aircraftCarrier) {
            if (orders.size() == 0) {
                Order o;
                o.type = Order::TYPE::MOVE;
								Order::Target t;
								t.unit = false;
								t.pos = new Vector3(lp);
                o.targets.push_back(t);
                setOrder(o);
            }

            if (!landing
                    &&aircraftCarrier
                    &&pos.getDistanceFrom(lp) <= unitData::destinationOffset[id]
                    &&*orders[0].targets[0].pos == lp){
                float angle = dirVec.getAngleBetween(aircraftCarrier->getDirVec()), maxAngle = PI - anglePrecision[id] / 180 * PI;

                if(!toTurnPoint && anglePrecision[id] / 180 * PI < angle && angle < maxAngle){
                    float radius = getCircleRadius();
                    Vector3 carrierSideVec = aircraftCarrier->getLeftVec().norm();
                    Vector3 jetSideVec = leftVec.norm();

                    if(carrierSideVec.getAngleBetween(dirVec) < PI / 2){
                        carrierSideVec = -carrierSideVec;
                        jetSideVec = -jetSideVec;
                    }

                    Vector3 tanPoint = pos + (jetSideVec + carrierSideVec) * radius;
                    float ang1 = (tanPoint - lp).getAngleBetween(carrierSideVec);
                    float ang2 = dirVec.getAngleBetween(-carrierSideVec);
                    float base = (tanPoint-lp).getLength() * cos(ang1);
                    float hyp = base / cos(ang2);
                    landingPos = (pos + dirVec * hyp);
                    toTurnPoint = true;
                }
                else if(PI - angle < anglePrecision[id] / 180 * PI){
                    onBoard = true;
                    landing = true;
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

    void Jet::lap(Vector3 destDir) {
        Vector3 center = pos - leftVec * getCircleRadius();
        Vector3 initCircleDir = -leftVec;
        float angle = initCircleDir.getAngleBetween(destDir);
        angle = (Quaternion(angle, upVec) * initCircleDir == destDir ? angle : 2 * PI - angle);
        Vector3 offsetDir = Quaternion(PI / 2, upVec) * destDir;
        Vector3 destDirPos = Quaternion(angle, upVec) * dirVec;
        Vector3 offsetDirPos = Quaternion(angle + PI / 2, upVec) * dirVec;
        Vector3 hypVec = *orders[0].targets[0].pos - destDirPos;
        float triAngle = hypVec.getAngleBetween(offsetDir);
        float distance = cos(triAngle) * hypVec.getLength();
        Vector3 destPoint = offsetDirPos + offsetDirPos.norm() * distance;
        float dirAngle = dirVec.getAngleBetween(offsetDir) * 180 / PI;
        float movementAmount;
        turn(maxTurnAngle > dirAngle ? dirAngle : maxTurnAngle);

        if(dirAngle > unitData::anglePrecision[id])
            movementAmount = speed;
        else{
            float destDistance = (destPoint - pos).getLength();
            movementAmount = (speed > destDistance ? destDistance : speed);
        }

        advance(movementAmount);
    }

    void Jet::takeOff() {
        float distance = aircraftCarrier->getRunwayLength() - (pos - aircraftCarrier->getJetPos(jetId)).getLength();
        float movementAmmount = speed > distance ? distance : speed;
        advance(movementAmmount);
        distance -= movementAmmount;

        if (distance <= 0.)
            onBoard = false;
    }

    void Jet::land(){
        float distance = (pos - aircraftCarrier->getJetPos(jetId)).getLength();
        float movementAmmount = (speed > distance ? distance : speed);
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
        float movementAmmount = (speed > distance ? distance : speed);
        advance(movementAmmount);
        distance -= movementAmmount;

        if (distance <= 0.)
            toTurnPoint = false;
    }

    void Jet::pitch(float angle) {
        Quaternion rotQuat = Quaternion(-angle / 180 * PI, leftVec);
        dirVec = rotQuat * dirVec, upVec = rotQuat * upVec;
        horAngle = angle;
        float yAngle = model->getOrientation().y;
				/*
        node->setOrientation(vector3df(angle, 0, 0));
        node->setOrientation(vector3df(angle, yAngle, 0));
				*/
    }
}
