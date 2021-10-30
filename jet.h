#pragma once
#ifndef JET_H
#define JET_H

#include "unit.h"
#include "player.h"
#include <time.h>
#include "util.h"

namespace game{
    namespace content{
        class AircraftCarrier;
        
        class Jet : public Unit {
        public:
            Jet(Player*,vector3df, int, bool);
//             virtual void attack(Order);
            virtual void update();
            inline void setJetId(int i){this->jetId=i;}
            inline void setAircraftCarrier(AircraftCarrier *a){aircraftCarrier=a;}
            inline bool isOnBoard(){return onBoard;}
            inline int getJetId(){return jetId;}
            inline AircraftCarrier* getAircraftCarrier(){return aircraftCarrier;}
            inline vector3df getOffsetPos(){return offsetPos;}
        private:
            int jetId;
            vector3df destDir,offsetPos;
            float horAngle = 0, pitchSpeed;
            void turnAround();
            void pitch(float);
            void lap(vector3df);
        protected:
            AircraftCarrier *aircraftCarrier=nullptr;
            vector3df landingPos;
            bool onBoard=true,toTurnPoint=false,landing=false;
            void move(Order, float = 0.);
            void takeOff();
            void land();
        };
    }
}

#endif
