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

		int Pathfinder::findMinDistVert(vector<Map::Cell> &cells, u32 distances[]){
			int numCells = cells.size(), minDistVert;

			for(int i = 0; i < numCells; i++)
				if(!cells[i].checked){
					minDistVert = i;
					break;
				}

			for(int i = 0; i < numCells; i++){
				bool checked = cells[i].checked;

				if(!checked && distances[i] < distances[minDistVert])
					minDistVert = i;
			}

			return minDistVert;
		}

		vector<int> Pathfinder::findPath(vector<Map::Cell> &cells, int source, int dest, int unitType){
			int size = cells.size();
			u32 distances[size];
			vector<int> paths[size];
			paths[source].push_back(source);

			for(int i = 0; i < size; i++){
				cells[i].checked = false;
				distances[i] = (i == source ? 0 : impassibleNodeVal);
			}

			while(!cells[dest].checked){
				int vertStrich = findMinDistVert(cells, distances);
				cells[vertStrich].checked = true;

				int numEdges = cells[vertStrich].edges.size();

				for(int i = 0; i < numEdges; i++){
					bool canMoveToStrichCell = true;
					bool ship = ((UnitType)unitType == UnitType::UNDERWATER || (UnitType)unitType == UnitType::SEA_LEVEL);

					if((ship && cells[vertStrich].type != Map::Cell::WATER) || ((UnitType)unitType == UnitType::LAND && cells[vertStrich].type != Map::Cell::LAND))
						canMoveToStrichCell = false;

					if(canMoveToStrichCell){
						int edgeNode = cells[vertStrich].edges[i].destCellId;
						bool isChecked = cells[edgeNode].checked;

						if(!isChecked && (distances[vertStrich] + cells[vertStrich].edges[i].weight < distances[edgeNode])){
							distances[edgeNode] = distances[vertStrich] + cells[vertStrich].edges[i].weight;
							paths[edgeNode] = paths[vertStrich];
							paths[edgeNode].push_back(edgeNode);
						}
					}
				}
			}

			for(int i = 0; i < size; i++)
				cells[i].checked = false;

			return paths[dest];
		}
}
