#include <algorithm>
#include <iostream>

#include "pathfinder.h"

namespace battleship{
		using namespace std;
		using namespace vb01;

		static Pathfinder *pathfinder = nullptr;

		Pathfinder* Pathfinder::getSingleton(){
				if(!pathfinder)
						pathfinder = new Pathfinder();

				return pathfinder;
		}

		Pathfinder::Pathfinder(){
		}

		vector<int> Pathfinder::findPath(vector<Map::Cell> &cells, int source, int dest){
			int size = cells.size();
				u32 distances[size];
				vector<int> paths[size];
				paths[source].push_back(source);

				for(int i = 0; i < size; i++)
					distances[i] = (i == source ? 0 : impassibleNodeVal);

				vector<int> checkedNodes;

				while(find(checkedNodes.begin(), checkedNodes.end(), dest) == checkedNodes.end()){
					bool initVertStrichSet = false;
					int vertStrich;

					for(int i = 0; i < size; i++){
						bool isChecked = find(checkedNodes.begin(), checkedNodes.end(), i) != checkedNodes.end();

						if(!isChecked){
							if(!initVertStrichSet){
								vertStrich = i;
								initVertStrichSet = true;
							}

							if(initVertStrichSet && (distances[vertStrich] > distances[i]))
								vertStrich = i;
						}
					}

					checkedNodes.push_back(vertStrich);

					for(int i = 0; i < size; i++){
						bool isChecked = find(checkedNodes.begin(), checkedNodes.end(), i) != checkedNodes.end();

						if(!isChecked && (distances[vertStrich] + cells[vertStrich].getEdgeWeight(i) < distances[i])){
							distances[i] = distances[vertStrich] + cells[vertStrich].getEdgeWeight(i);
							paths[i] = paths[vertStrich];
							paths[i].push_back(i);
						}
					}
				}

				return paths[dest];
		}
}
