#include "exitButton.h"

using namespace irr::core;
using namespace game::core;

namespace game
{
    namespace gui{
        ExitButton::ExitButton(GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
        }

        void ExitButton::onClick() {
            gameManager->getDevice()->closeDevice();
        }
    }
}
