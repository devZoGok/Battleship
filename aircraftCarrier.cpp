#include "aircraftCarrier.h"
#include "aircraftCarrierData.h"
#include "missileJet.h"
#include "demoJet.h"

using namespace game::core;

namespace game{
    namespace content{
        AircraftCarrier::AircraftCarrier(GameManager *gM, Player *player,vector3df pos, int unitId) : Vessel(gM, player,pos, unitId) {
            maxNumJets=unitData::numJets[unitId];
            jets=new Jet*[maxNumJets];
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
                    j = (MissileJet*)new MissileJet(gameManager, player,getJetPos(slot), id);
                } else {
                    id = 10;
                    j = (DemoJet*)new DemoJet(gameManager, player,getJetPos(slot), id);
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
                if (j&&j->isOnBoard()) {
                    vector3df oP = j->getOffsetPos();
                    j->orientUnit(dirVec);
                    j->placeUnit(pos + oP.X * leftVec + oP.Y * upVec - oP.Z * dirVec);
                }
            }
        }
    }
}
