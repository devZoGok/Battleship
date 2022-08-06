#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

#include <util.h>

namespace battleship{
		class Pathfinder{
				public:
						static Pathfinder* getSingleton();
						std::vector<int> findPath(vb01::s32**, int, int, int);
				private:
						Pathfinder();
		};
}

#endif
