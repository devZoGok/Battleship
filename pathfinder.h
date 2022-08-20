#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

#include <util.h>

namespace battleship{
		class Pathfinder{
				public:
					static Pathfinder* getSingleton();
					std::vector<int> findPath(vb01::u32**, int, int, int);
					void generateWeights(vb01::u32**, int&, vb01::Vector3*[], vb01::Vector3);
				private:
					Pathfinder();
		};
}

#endif
