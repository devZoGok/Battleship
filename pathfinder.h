#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"

#include <vector>

#include <util.h>

namespace battleship{
	class Pathfinder{
		public:
			static Pathfinder* getSingleton();
			std::vector<int> findPath(std::vector<Map::Cell>&, int, int, int = -1);
			inline vb01::u32 getImpassibleNodeVal(){return impassibleNodeVal;}
			inline void setImpassibleNodeVal(vb01::u32 val){this->impassibleNodeVal = val;}
		private:
			Pathfinder(){}
			int findMinDistVert(std::vector<std::pair<int, bool>>&, vb01::u32[]);

			vb01::u32 impassibleNodeVal;
	};
}

#endif
