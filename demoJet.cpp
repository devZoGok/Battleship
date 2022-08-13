#include <quaternion.h>

#include <stateManager.h>

#include "demoJet.h"
#include "inGameAppState.h"
#include "util.h"

using namespace vb01;

namespace battleship{
    DemoJet::DemoJet(Player *player, Vector3 pos, int id, bool onBoard) : Jet(player, pos, id,onBoard) {}
    
    void DemoJet::attack(Order order){
        if(!onBoard){
            Unit::attack(order);
            Vector3 target = order.targets[0].pos;
            float angle = dirVec.getAngleBetween(target - pos);
            Vector3 axis = dirVec.cross(target - pos);

            if(maxTurnAngle < angle / PI * 180){
                Quaternion rotQuat = Quaternion(maxTurnAngle / 180 * PI, axis);
                orientUnit(rotQuat*dirVec);
            }
        }
        else
            takeOff();
    }
    
    void DemoJet::update(){
        Jet::update();
    }
}
