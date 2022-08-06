#include "pathfinderTest.h"
#include "pathfinder.h"

#include <util.h>

#include <vector>

namespace battleship{
		using namespace std;
		using namespace vb01;

		void PathfinderTest::testFindPath(){
				int size = 7;
				s32 w[size][size] = {
						{0, 2, 4, 999, 999, 999, 999},
						{999, 0, 1, 9, 13, 999, 999},
						{999, 2, 0, 999, 4, 5, 999},
						{999, 999, 999, 0, 999, 999, 1},
						{1, 999, 999, 1, 0, 999, 3},
						{999, 999, 999, 999, 9, 0, 2},
						{999, 999, 999, 999, 999, 999, 0}
				};
				s32 **weights = new s32*[size];

				for(int i = 0; i < size; i++){
						weights[i] = new s32[size];

						for(int j = 0; j < size; j++)
								weights[i][j] = w[i][j];
				}

				vector<int> path = pathfinder->findPath(weights, size, 0, size - 1);
				CPPUNIT_ASSERT(path == vector<int>({0, 1, 2, 4, 3, 6}));

				int sumPathWeights = 0;

				for(int i = 1; i < size; i++)
						sumPathWeights += w[path[i - 1]][path[i]];

				CPPUNIT_ASSERT(sumPathWeights == 9);
		}

		void PathfinderTest::setUp(){
				pathfinder = Pathfinder::getSingleton();
		}

		void PathfinderTest::tearDown(){
		}
}
