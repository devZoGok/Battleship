#include "unitListbox.h"
#include "unit.h"
#include "unitFrameController.h"

#include <luaManager.h>

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
		LuaManager *lm = LuaManager::getSingleton();
		string modelPath = lm->getStringFromTable("basePath", vector<Index>{Index(id + 1)}) + lm->getStringFromTable("meshPath", vector<Index>{Index(id + 1)});
		ufCtr->addUnitFrame(UnitFrameController::UnitFrame(modelPath, id, (int)UnitType::LAND));

		ufCtr->setPlacingFrames(true);
	}
}
