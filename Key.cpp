#include"Key.h"

namespace game{
    namespace core{
        Key::Key(Bind bind, int trigger, bool key, bool analog) {
            this->bind = bind;
            this->trigger = trigger;
            this->key = key;
            this->analog = analog;
            beingUsed = false;
        }
    }
}
