#include "exitButton.h"
#include <root.h>

using namespace std;
using namespace vb01;

namespace battleship{
    ExitButton::ExitButton(Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, separate) {
    }

    void ExitButton::onClick() {
				//Root::getSingleton()->set
    }
}
