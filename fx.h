#ifndef FX_H
#define FX_H

#include <util.h>

namespace sf{
	class Sound;
}

namespace battleship{
    struct Fx {
		vb01::s64 initTime, time;
        sf::Sound *sfx = nullptr;
    };
}

#endif
