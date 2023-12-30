#include "buildButton.h"
#include "gameManager.h"
#include "gameObject.h"
#include "gameObjectFrameController.h"

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
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();
		ufCtr->addGameObjectFrame(GameObjectFrame(structureId, GameObject::Type::UNIT));
		ufCtr->setPlacingFrames(true);
	}
}
