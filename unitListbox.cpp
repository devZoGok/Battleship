#include <solUtil.h>

#include "unitListbox.h"
#include "unit.h"
#include "gameObjectFrameController.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;
	
	UnitListbox::UnitListbox(Vector2 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath){
	}

	void UnitListbox::onClose(){
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();
		ufCtr->addGameObjectFrame(GameObjectFrame(selectedOption, GameObjectFrame::UNIT));
		ufCtr->setPlacingFrames(true);
	}
}
