#include <solUtil.h>

#include "unitListbox.h"
#include "unit.h"
#include "unitFrameController.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;
	
	UnitListbox::UnitListbox(Vector2 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath){
	}

	void UnitListbox::onClose(){
		UnitFrameController *ufCtr = UnitFrameController::getSingleton();
		
		int id = selectedOption;
		sol::state_view SOL_LUA_STATE = generateView();
		string modelPath = (string)SOL_LUA_STATE["basePath"][id + 1] + (string)SOL_LUA_STATE["meshPath"][id + 1];
		ufCtr->addUnitFrame(UnitFrameController::UnitFrame(modelPath, id, (int)UnitType::LAND));

		ufCtr->setPlacingFrames(true);
	}
}
