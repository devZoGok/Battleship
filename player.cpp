#include <solUtil.h>

#include "player.h"
#include "structure.h"
#include "stateManager.h"
#include "activeGameState.h"
#include "resourceDeposit.h"

namespace battleship{
	using namespace vb01;
	using namespace gameBase;
	using namespace std;

    Player::Player(int diff, int fac, int t, bool cpuPl, Vector3 sp) : difficulty(diff), faction(fac), team(t), cpuPlayer(cpuPl), spawnPoint(sp) {}

    Player::~Player() {}

    void Player::update() {
		for(Unit *u : units)
			u->update();

		for(ResourceDeposit *rd : resourceDeposits)
			rd->update();
    }

	int Player::getOrderLineId(Order::TYPE type, Vector3 startPos, Vector3 endPos){
		Vector3 color;

      	switch(type){
      	    case Order::TYPE::MOVE:
				color = Vector3::VEC_J;
      	        break;
      	    case Order::TYPE::ATTACK:
				color = Vector3::VEC_I;
      	        break;
      	    case Order::TYPE::PATROL:
      	    case Order::TYPE::GARRISON:
      	    case Order::TYPE::EJECT:
				color = Vector3::VEC_K;
				break;
      	    case Order::TYPE::BUILD:
				color = Vector3(1, 1, 0);
      	        break;
      	}

		LineRenderer *lineRenderer = LineRenderer::getSingleton();
		lineRenderer->addLine(startPos, endPos, color);
		vector<LineRenderer::Line> lines = lineRenderer->getLines();

		return lines[lines.size() - 1].id;
	}

	void Player::issueOrder(Order::TYPE type, Vector3 destDir, vector<Order::Target> targets, bool append){
		if(type != Order::TYPE::EJECT && targets.empty()) return;

		vector<Unit*> selectedUnits = getSelectedUnits();

		if(type == Order::TYPE::EJECT){
			for(Unit *u : selectedUnits){
				const vector<Unit::GarrisonSlot> &garrisonSlots = u->getGarrisonSlots();

				for(Unit::GarrisonSlot slot : garrisonSlots)
					if(slot.vehicle)
						targets.push_back(Order::Target((Unit*)slot.vehicle));
			}
		}
		else if(type != Order::TYPE::PATROL)
			targets.push_back(Order::Target());

        for (Unit *u : selectedUnits) {
			bool targetingSelf = false, structBuilt = true;

			for(Order::Target &targ : targets){
				if(targ.unit && targ.unit == u){
					targetingSelf = true;
					break;
				}

				if(!u->isVehicle() && ((Structure*)u)->getBuildStatus() < 100){
					structBuilt = false;
					break;
				}
			}

			if(targetingSelf || !structBuilt) continue;

			int lineId = -1;
			ActiveGameState *activeState = ((ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE));

			if(activeState && activeState->getPlayer() == this)
				lineId = getOrderLineId(type, u->getPos(), targets[0].pos);

        	Order order(type, targets, destDir, lineId);

            if (append)
                u->addOrder(order);
            else
                u->setOrder(order);
        }
	}

    bool Player::isThisPlayersUnit(GameObject *u) {
        bool foundUnit = false;

        if (units.size() > 0) {
            for (int i = 0; i < units.size() && !foundUnit; i++)
                if (units[i] == u)
                    foundUnit = true;

            return foundUnit;
        } else
            return false;
    }

	void Player::deselectUnits(){
		vector<Unit*> selectedUnits = getSelectedUnits();

		for(Unit *u : selectedUnits)
			u->toggleSelection(false);
	}

	void Player::removeUnit(Unit *unit){
		for(int i = 0; i < units.size(); i++)
			if(unit == units[i]){
				removeUnit(i);
				break;
			}
	}

	void Player::removeUnit(int id){
		if(units[id]->isSelected())
			units[id]->toggleSelection(false);

		delete units[id];
		units.erase(units.begin() + id);
	}

	vector<Unit*> Player::getSelectedUnits(){
		vector<Unit*> selectedUnits;

		for(Unit *u : units)
			if(u->isSelected())
				selectedUnits.push_back(u);

		return selectedUnits;
	}

	Unit* Player::getSelectedUnit(int selectedUnitId){
		Unit *selectedUnit = nullptr;

		for(int i = 0, selUnitId = 0; i < units.size(); i++)
			if(units[i]->isSelected()){
				if(selUnitId == selectedUnitId){
					selectedUnit = units[i];
					break;
				}

				selUnitId++;
			}

		return selectedUnit;
	}

	vector<Unit*> Player::getSelectedUnitsByClass(UnitClass uc){
		vector<Unit*> selectedUnits = getSelectedUnits(), unitsByClass;

		for(Unit *u : selectedUnits)
			if(u->getUnitClass() == uc)
				unitsByClass.push_back(u);

		return unitsByClass;
	}

	void Player::selectUnits(vector<Unit*> selUnits){
		for(Unit *u : selUnits)
			u->toggleSelection(true);
	}
}
