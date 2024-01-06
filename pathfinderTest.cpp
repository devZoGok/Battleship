#include "pathfinderTest.h"
#include "pathfinder.h"
#include "map.h"

#include <util.h>

#include <vector>

namespace battleship{
		using namespace std;
		using namespace vb01;

		void PathfinderTest::testFindPath(){
			vector<Map::Cell> cells = vector<Map::Cell>{
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 0, 0), Map::Edge(2, 0, 1), Map::Edge(4, 0, 2)}),
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 1, 1), Map::Edge(1, 1, 2), Map::Edge(9, 1, 3), Map::Edge(13, 1, 4)}),
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 2, 2), Map::Edge(2, 2, 1), Map::Edge(4, 2, 4), Map::Edge(5, 2, 5)}),
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 3, 3), Map::Edge(1, 3, 6)}),
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 4, 4), Map::Edge(1, 4, 1), Map::Edge(1, 4, 3), Map::Edge(2, 4, 5), Map::Edge(3, 4, 6)}),
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 5, 5), Map::Edge(9, 5, 4), Map::Edge(2, 5, 6)}),
				Map::Cell(Vector3::VEC_ZERO, Map::Cell::Type::LAND, vector<Map::Edge>{Map::Edge(0, 6, 6)})
			};

			int size = cells.size();
			const u16 INF = u16(0 - 1);
			pathfinder->setImpassibleNodeVal(INF);
			vector<int> path = pathfinder->findPath(cells, 0, size - 1);
			CPPUNIT_ASSERT(path == vector<int>({0, 1, 2, 4, 3, 6}));

			int sumPathWeights = 0;

			for(int i = 0; i < path.size() - 1; i++)
				for(int j = 0; j < cells[path[i]].edges.size(); j++)
					if(cells[path[i]].edges[j].destCellId == path[i + 1])
						sumPathWeights += cells[path[i]].edges[j].weight;

			CPPUNIT_ASSERT(sumPathWeights == 9);
		}

		void PathfinderTest::testFindBigPath(){
		}

		void PathfinderTest::setUp(){
				pathfinder = Pathfinder::getSingleton();
		}

		void PathfinderTest::tearDown(){
		}
}
