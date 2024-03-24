#include "trainButton.h"
#include "activeGameState.h"
#include "buildableUnit.h"
#include "factory.h"

#include <stateManager.h>
#include <solUtil.h>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace gameBase;

	TrainButton::TrainButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath, int uid, int slId) :
		UnitButton(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, imagePath, uid),
		slotId(slId) {}

	void TrainButton::onClick(){
		vector<Unit*> labs = getUnits(unitId);

		for(Unit *lab : labs)
			((Factory*)lab)->appendToQueue(slotId);
	}
}
