#include "unitButton.h"

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace vb01Gui;

	UnitButton::UnitButton(Vector2 pos, Vector2 size, string name, string fontPath, int trigger, string imagePath) : Button(pos, size, name, fontPath, true, trigger, imagePath){}
}
