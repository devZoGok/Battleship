#include <algorithm>

#include "pathfinder.h"
#include "unit.h"

namespace battleship{
		using namespace std;
		using namespace vb01;

		static Pathfinder *pathfinder = nullptr;

		Pathfinder* Pathfinder::getSingleton(){
			if(!pathfinder)
				pathfinder = new Pathfinder();

			return pathfinder;
		}

		int Pathfinder::findMinDistVert(vector<pair<int, bool>> &cells, u32 distances[]){
			int numCells = cells.size(), minDistVert;

			for(int i = 0; i < numCells; i++)
				if(!cells[i].second){
					minDistVert = i;
					break;
				}

			for(int i = 0; i < numCells; i++){
				bool checked = cells[i].second;

				if(!checked && distances[i] < distances[minDistVert])
					minDistVert = i;
			}

			return minDistVert;
		}

		vector<int> Pathfinder::findPath(vector<Map::Cell> &cells, int source, int dest, int unitType){
			const int size = cells.size();
			u32 *distances = new u32[size];
			vector<int> *paths = new vector<int>[size];
			paths[source].push_back(source);
			vector<pair<int, bool>> cellsByCheck;

			for(int i = 0; i < size; i++){
				cellsByCheck.push_back(pair(i, false));
				distances[i] = (i == source ? 0 : impassibleNodeVal);
			}

			while(!cellsByCheck[dest].second){
				int vertStrich = findMinDistVert(cellsByCheck, distances);
				cellsByCheck[vertStrich].second = true;

				int numEdges = cells[vertStrich].edges.size();

				for(int i = 0; i < numEdges; i++){
					bool canMoveToStrichCell = true;
					bool ship = ((UnitType)unitType == UnitType::UNDERWATER || (UnitType)unitType == UnitType::SEA_LEVEL);

					if((ship && cells[vertStrich].type != Map::Cell::WATER) || ((UnitType)unitType == UnitType::LAND && cells[vertStrich].type != Map::Cell::LAND))
						canMoveToStrichCell = false;

					int edgeNode = cells[vertStrich].edges[i].destCellId, cellTypeFactor = (canMoveToStrichCell ? 1 : 100);
					bool isChecked = cellsByCheck[edgeNode].second;

					if(!isChecked && (distances[vertStrich] + cells[vertStrich].edges[i].weight * cellTypeFactor < distances[edgeNode])){
						distances[edgeNode] = distances[vertStrich] + cells[vertStrich].edges[i].weight * cellTypeFactor;
						paths[edgeNode] = paths[vertStrich];
						paths[edgeNode].push_back(edgeNode);
					}
				}
			}

			vector<int> path = paths[dest];

			delete[] paths;
			delete[] distances;

			return path;
		}
}
