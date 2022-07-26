#include "pathfinder.h"

namespace battleship{
		using namespace std;

		static Pathfinder *pathfinder = nullptr;

		Pathfinder* Pathfinder::getSingleton(){
				if(!pathfinder)
						pathfinder = new Pathfinder();

				return pathfinder;
		}

		Pathfinder::Pathfinder(){
		}

		vector<int> Pathfinder::findPath(int **weights, int size, int source, int sink){
				int pathWeights[size];
				pathWeights[0] = 0;

				for(int i = 1; i < size; i++)
						pathWeights[i] = -1;

				vector<int> path;
				return path;
		}
}
