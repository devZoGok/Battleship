#pragma once
#ifndef AIRCRAFT_CARRIER_H
#define AIRCRAFT_CARRIER_H

#include "vessel.h"
#include "jet.h"
#include "aircraftCarrierData.h"

namespace battleship{
    class AircraftCarrier : public Vessel {
    public:
        AircraftCarrier(Player*, vb01::Vector3, int);
        ~AircraftCarrier();
        void makeJet();
        inline void setJet(int i, Jet *j){this->jets[i]=j;}
        inline int getMaxNumJets(){return maxNumJets;}
        inline Jet* getJet(int i){return jets[i];}
        inline Jet** getJets(){return jets;}
        inline vb01::Vector3 getJetPos(int i){return pos + leftVec * unitData::jetPos[id][i].x - dirVec * unitData::jetPos[id][i].z + upVec * unitData::jetPos[id][i].y;}
        inline float getRunwayLength(){return runwayLength;}
    private:
        Jet** jets;
        int maxNumJets;
        float runwayLength;
        void move(Order, float = 0.);
    };
}

#endif
