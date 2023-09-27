#include "buildButton.h"
#include "gameManager.h"
#include "unit.h"
#include "unitFrameController.h"

#include <solUtil.h>

namespace battleship{
	using namespace sol;
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;

	BuildButton::BuildButton(Vector2 pos, Vector2 size, int structureId, string name, int trigger, string imagePath) : UnitButton(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, imagePath){
		this->structureId = structureId;
	}

	void BuildButton::onClick(){
		sol::state_view SOL_LUA_STATE = generateView();
		string modelPath = (string)SOL_LUA_STATE["basePath"][structureId + 1] + (string)SOL_LUA_STATE["meshPath"][structureId + 1];

		UnitFrameController *ufCtr = UnitFrameController::getSingleton();
		ufCtr->addUnitFrame(UnitFrameController::UnitFrame(modelPath, structureId, (int)UnitType::LAND));
		ufCtr->setPlacingFrames(true);
	}
}
