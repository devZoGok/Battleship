#include "pathfinderTest.h"
#include "pathfinder.h"

#include <util.h>

#include <vector>

namespace battleship{
		using namespace std;
		using namespace vb01;

		void PathfinderTest::testFindPath(){
				int size = 7;
				const u16 INF = u16(0 - 1);
				//pathfinder->setImpassibleNodeVal(INF);
				u32 w[size][size] = {
						{0, 2, 4, INF, INF, INF, INF},
						{INF, 0, 1, 9, 13, INF, INF},
						{INF, 2, 0, INF, 4, 5, INF},
						{INF, INF, INF, 0, INF, INF, 1},
						{1, INF, INF, 1, 0, INF, 3},
						{INF, INF, INF, INF, 9, 0, 2},
						{INF, INF, INF, INF, INF, INF, 0}
				};
				u32 **weights = new u32*[size];

				for(int i = 0; i < size; i++){
						weights[i] = new u32[size];

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
