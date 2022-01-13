#pragma once
#ifndef JET_H
#define JET_H

#include <time.h>

#include "unit.h"
#include "player.h"
#include "util.h"

namespace battleship{
    class AircraftCarrier;
    
    class Jet : public Unit {
    public:
        Jet(Player*, vb01::Vector3, int, bool);
        virtual void update();
        inline void setJetId(int i){this->jetId = i;}
        inline void setAircraftCarrier(AircraftCarrier *a){aircraftCarrier = a;}
        inline bool isOnBoard(){return onBoard;}
        inline int getJetId(){return jetId;}
        inline AircraftCarrier* getAircraftCarrier(){return aircraftCarrier;}
        inline vb01::Vector3 getOffsetPos(){return offsetPos;}
    private:
        int jetId;
				vb01::Vector3 destDir,offsetPos;
        float horAngle = 0, pitchSpeed;
        void turnAround();
        void pitch(float);
        void lap(vb01::Vector3);
    protected:
        AircraftCarrier *aircraftCarrier = nullptr;
				vb01::Vector3 landingPos;
        bool onBoard = true, toTurnPoint = false, landing = false;
        void move(Order, float = 0.);
        void takeOff();
        void land();
    };
}

#endif
