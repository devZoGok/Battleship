#include "unitDataManager.h"
#include "defConfigs.h"

#include <luaManager.h>

#include <string>
#include <iostream>

namespace battleship{
		using namespace std;
		using namespace vb01;
		using namespace gameBase;

		UnitDataManager *unitDataManager = nullptr;

		UnitDataManager* UnitDataManager::getSingleton(){
				if(!unitDataManager)
						unitDataManager = new UnitDataManager();

				return unitDataManager;
		}

		UnitDataManager::UnitDataManager(){
				LuaManager *luaManager = LuaManager::getSingleton();
				string path = GameManager::getSingleton()->getPath();
				luaManager->buildScript(vector<string>{path + "Scripts/defPaths.lua", path + "Scripts/unitData.lua"});

				numUnits = luaManager->getInt("numUnits");

				health = new int[numUnits];
				speed = new float[numUnits];
				destinationOffset = new float[numUnits];
				unitType = new UnitType[numUnits];
				unitClass = new UnitClass[numUnits];
				anglePrecision = new float[numUnits];
				cost = new int[numUnits];
				maxTurnAngle = new float[numUnits];
				range = new float[numUnits];
				unitAxisLength = new float[numUnits];
				lineOfSight = new float[numUnits];
				name = new string[numUnits];
				meshPath = new string[numUnits];
				basePath = new string[numUnits];
				unitCornerPoints = new Vector3*[numUnits];
				unitCuboidDimensions = new Vector3*[numUnits];

				for(int i = 0; i < numUnits; i++){
					health[i] = luaManager->getIntFromTable("health", vector<Index>{Index(i + 1)});
					speed[i] = luaManager->getFloatFromTable("speed", vector<Index>{Index(i + 1)});
					unitClass[i] = (UnitClass)luaManager->getIntFromTable("unitClass", vector<Index>{Index(i + 1)});
					destinationOffset[i] = luaManager->getFloatFromTable("destinationOffset", vector<Index>{Index(i + 1)});
					unitType[i] = (UnitType)luaManager->getIntFromTable("unitType", vector<Index>{Index(i + 1)});
					anglePrecision[i] = luaManager->getFloatFromTable("anglePrecision", vector<Index>{Index(i + 1)});
					cost[i] = luaManager->getIntFromTable("cost", vector<Index>{Index(i + 1)});
					maxTurnAngle[i] = luaManager->getFloatFromTable("maxTurnAngle", vector<Index>{Index(i + 1)});
					range[i] = luaManager->getFloatFromTable("range", vector<Index>{Index(i + 1)});
					unitAxisLength[i] = luaManager->getFloatFromTable("unitAxisLength", vector<Index>{Index(i + 1)});
					lineOfSight[i] = luaManager->getFloatFromTable("lineOfSight", vector<Index>{Index(i + 1)});
					name[i] = luaManager->getStringFromTable("name", vector<Index>{Index(i + 1)});
					meshPath[i] = luaManager->getStringFromTable("meshPath", vector<Index>{Index(i + 1)});
					basePath[i] = luaManager->getStringFromTable("basePath", vector<Index>{Index(i + 1)});

					unitCornerPoints[i] = new Vector3[8];
					unitCuboidDimensions[i] = new Vector3[8];

					for(int j = 0; j < 8; j++){
							float cornerX = luaManager->getFloatFromTable("unitCornerPoints", vector<Index>{Index(i + 1), Index(j + 1), Index("x")});
							float cornerY = luaManager->getFloatFromTable("unitCornerPoints", vector<Index>{Index(i + 1), Index(j + 1), Index("y")});
							float cornerZ = luaManager->getFloatFromTable("unitCornerPoints", vector<Index>{Index(i + 1), Index(j + 1), Index("z")});
							unitCornerPoints[i][j] = Vector3(cornerX, cornerY, cornerZ);

							float dimX = luaManager->getFloatFromTable("unitCuboidDimensions", vector<Index>{Index(i + 1), Index(j + 1), Index("x")});
							float dimY = luaManager->getFloatFromTable("unitCuboidDimensions", vector<Index>{Index(i + 1), Index(j + 1), Index("y")});
							float dimZ = luaManager->getFloatFromTable("unitCuboidDimensions", vector<Index>{Index(i + 1), Index(j + 1), Index("z")});
							unitCuboidDimensions[i][j] = Vector3(dimX, dimY, dimZ);
					}
				}
		}
}
