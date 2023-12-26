#include "statsButton.h"
#include "gameManager.h"
#include "concreteGuiManager.h"

namespace battleship{
	using namespace std;
	using namespace vb01;

	StatsButton::StatsButton(Vector2 pos, Vector2 size, string name, int trigger, string imagePath) : Button(pos, size, name, GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", trigger, true, imagePath){}

	void StatsButton::onClick(){
		ConcreteGuiManager::getSingleton()->readLuaScreenScript("statistics.lua");
	}
}
