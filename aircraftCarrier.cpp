#include "aircraftCarrier.h"
#include "aircraftCarrierData.h"
#include "missileJet.h"
#include "demoJet.h"

using namespace vb01;
using namespace std;

namespace battleship{
    AircraftCarrier::AircraftCarrier(Player *player, Vector3 pos, int unitId) : Vessel(player,pos, unitId) {
        maxNumJets = unitData::numJets[unitId];
        jets = new Jet*[maxNumJets];

        for(int i=0;i<maxNumJets;i++)
            jets[i]=nullptr;

        runwayLength=unitData::runwayLenght[unitId];

        for (int i = 0; i < unitData::numJets[unitId]; i++)
            makeJet();
    }

    AircraftCarrier::~AircraftCarrier(){
        delete[] jets;
    }
    
    void AircraftCarrier::makeJet(){
        int slot=-1;

        for(int i=0; i<unitData::numJets[id]&&slot==-1; i++)
            if(!jets[i])
                slot=i;

        if(slot!=-1){
            int id = 0;
            Jet *j = nullptr;

            if (this->id == 6) {
                id = 9;
                j = (MissileJet*)new MissileJet(player,getJetPos(slot), id);
            } else {
                id = 10;
                j = (DemoJet*)new DemoJet(player,getJetPos(slot), id);
            }

            j->setJetId(slot);
            jets[slot]=j;
            j->setAircraftCarrier(this);
            player->addUnit(j);
        }
    }
    
    void AircraftCarrier::move(Order order, float offset) {
        Unit::move(order, offset);

        for (int i=0;i<maxNumJets;i++) {
            Jet *j=jets[i];

            if (j && j->isOnBoard()) {
                Vector3 oP = j->getOffsetPos();
                //j->orientUnit(dirVec);
                j->placeUnit(pos + leftVec * oP.x + upVec * oP.y - dirVec * oP.z);
            }
        }
    }
}
