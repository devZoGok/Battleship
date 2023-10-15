#ifndef FX_H
#define FX_H

#include <util.h>

namespace sf{
	class Sound;
}

namespace vb01{
	class Node;
}

namespace battleship{
    struct Fx {
		vb01::s64 initTime, time;
        sf::Sound *sfx = nullptr;
		vb01::Node *peNode = nullptr;

		Fx(vb01::s64 t, sf::Sound *s = nullptr, vb01::Node *n = nullptr) : time(t), sfx(s), peNode(n), initTime(vb01::getTime()){}
		void activate();
    };
}

#endif
