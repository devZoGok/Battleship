#include "minimapButton.h"
#include "map.h"

#include <node.h>

namespace battleship{
	using namespace vb01;
	using namespace std;

	MinimapButton::MinimapButton(Vector3 pos, Vector2 size, string ip) : Button(pos, size, "minimap", "", -1, true, ip){
		content = new Node();
		rectNode->attachChild(content);
	}

	MinimapButton::~MinimapButton(){
	}

	void MinimapButton::update(){
		Button::update();
	}

	void MinimapButton::onClick(){
		Vector2 clickPos = getCursorPos();
		float posXRatio = (clickPos.x - pos.x) / size.x;
		float posYRatio = (clickPos.y - pos.y) / size.y;

		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 mapSize = Map::getSingleton()->getMapSize();
		cam->setPosition(Vector3(mapSize.x * (-.5 + posXRatio), cam->getPosition().y, mapSize.z * (-.5 + posYRatio)));
	}
}
