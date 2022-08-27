#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

#include <util.h>

namespace battleship{
		class Pathfinder{
				public:
					static Pathfinder* getSingleton();
					std::vector<int> findPath(vb01::u32**, int, int, int);
					inline vb01::u32 getImpassibleNodeVal(){return impassibleNodeVal;}
					inline void setImpassibleNodeVal(vb01::u32 val){this->impassibleNodeVal = val;}
				private:
					Pathfinder();

					vb01::u32 impassibleNodeVal;
		};
}

#endif
