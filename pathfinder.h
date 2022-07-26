#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

namespace battleship{
		class Pathfinder{
				public:
						static Pathfinder* getSingleton();
						std::vector<int> findPath(int**, int, int, int);
				private:
						Pathfinder();
		};
}

#endif
