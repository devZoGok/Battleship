#include <solUtil.h>

#include "gameObject.h"
#include "gameObjectListbox.h"
#include "gameObjectFrameController.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;
	using namespace gameBase;
	
	GameObjectListbox::GameObjectListbox(bool ul, Vector3 pos, Vector2 size, vector<string> lines, int maxDisplay, string fontPath) : Listbox(pos, size, lines, maxDisplay, fontPath), unitListbox(ul) {}

	void GameObjectListbox::onClose(){
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();
		GameObject::Type type = (unitListbox ? GameObject::Type::UNIT : GameObject::Type::RESOURCE_DEPOSIT);
		ufCtr->addGameObjectFrame(GameObjectFrame(selectedOption, type));
		ufCtr->setPlacingFrames(true);
	}
}
