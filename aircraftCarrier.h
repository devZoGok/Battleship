#pragma once
#ifndef AIRCRAFT_CARRIER_H
#define AIRCRAFT_CARRIER_H

#include "vessel.h"
#include "jet.h"
#include "aircraftCarrierData.h"

namespace game{
    namespace content{
        class AircraftCarrier : public Vessel {
        public:
            AircraftCarrier(core::GameManager*, Player*, irr::core::vector3df, int);
            ~AircraftCarrier();
            void makeJet();
            inline void setJet(int i, Jet *j){this->jets[i]=j;}
            inline int getMaxNumJets(){return maxNumJets;}
            inline Jet* getJet(int i){return jets[i];}
            inline Jet** getJets(){return jets;}
            inline irr::core::vector3df getJetPos(int i){return pos+unitData::jetPos[id][i].X * leftVec - unitData::jetPos[id][i].Z * dirVec + unitData::jetPos[id][i].Y*upVec;}
            inline float getRunwayLength(){return runwayLength;}
        private:
            Jet** jets;
            int maxNumJets;
            float runwayLength;
            void move(Order, float = 0.);
        };
    }
}

#endif
