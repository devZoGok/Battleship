#include <algorithm>
#include <iostream>

#include "pathfinder.h"
#include "map.h"

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

		vector<int> Pathfinder::findPath(u32 **weights, int size, int source, int dest){
				u32 distances[size];
				vector<int> paths[size];
				paths[source].push_back(source);

				for(int i = 0; i < size; i++){
					distances[i] = (i == source ? 0 : impassibleNodeVal);

					for(int j = 0; j < size; j++)
						if(weights[i][j] > impassibleNodeVal){
							cout << "Node val higher than allowed max value\n";
							exit(-1);
						}
				}

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

						if(!isChecked && (distances[vertStrich] + weights[vertStrich][i] < distances[i])){
							distances[i] = distances[vertStrich] + weights[vertStrich][i];
							paths[i] = paths[vertStrich];
							paths[i].push_back(i);
						}
					}
				}

				return paths[dest];
		}
}