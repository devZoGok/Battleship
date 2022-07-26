#include "pathfinderTest.h"
#include "pathfinder.h"

#include <vector>

namespace battleship{
		using namespace std;

		void PathfinderTest::testFindPath(){
				int size = 7;
				int w[size][size] = {
						{0, 2, 4, -1, -1, -1, -1},
						{-1, 0, 1, 9, 13, -1, -1},
						{-1, 2, 0, -1, 4, 5, -1},
						{-1, -1, -1, 0, -1, -1, 1},
						{1, -1, -1, 1, 0, -1, 3},
						{-1, -1, -1, -1, 9, 0, 2},
						{-1, -1, -1, -1, -1, -1, 0}
				};
				int **weights = new int*[size];

				for(int i = 0; i < size; i++){
						weights[i] = new int[size];

						for(int j = 0; j < size; j++)
								weights[i][j] = w[i][j];
				}

				vector<int> path = pathfinder->findPath(weights, size, 0, size - 1);
				CPPUNIT_ASSERT(path.size() == 6);

				int sumPathWeights = 0;

				for(int p : path)
						sumPathWeights += p;

				CPPUNIT_ASSERT(sumPathWeights == 9);
		}

		void PathfinderTest::setUp(){
				pathfinder = Pathfinder::getSingleton();
		}

		void PathfinderTest::tearDown(){
		}
}
