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
				luaManager->buildScript(vector<string>{configData::PATH + "Scripts/defPaths.lua", configData::PATH + "Scripts/unitData.lua"});

				numUnits = luaManager->getInt("numUnits");

				health = new int[numUnits];
				speed = new float[numUnits];
				destinationOffset = new float[numUnits];
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
					health[i] = luaManager->getIntFromTable("health", vector<Index>{Index(to_string(i + 1), false)});
					speed[i] = luaManager->getFloatFromTable("speed", vector<Index>{Index(to_string(i + 1), false)});
					destinationOffset[i] = luaManager->getFloatFromTable("destinationOffset", vector<Index>{Index(to_string(i + 1), false)});
					anglePrecision[i] = luaManager->getFloatFromTable("anglePrecision", vector<Index>{Index(to_string(i + 1), false)});
					cost[i] = luaManager->getIntFromTable("cost", vector<Index>{Index(to_string(i + 1), false)});
					maxTurnAngle[i] = luaManager->getFloatFromTable("maxTurnAngle", vector<Index>{Index(to_string(i + 1), false)});
					range[i] = luaManager->getFloatFromTable("range", vector<Index>{Index(to_string(i + 1), false)});
					unitAxisLength[i] = luaManager->getFloatFromTable("unitAxisLength", vector<Index>{Index(to_string(i + 1), false)});
					lineOfSight[i] = luaManager->getFloatFromTable("lineOfSight", vector<Index>{Index(to_string(i + 1), false)});
					name[i] = luaManager->getStringFromTable("name", vector<Index>{Index(to_string(i + 1), false)});
					meshPath[i] = luaManager->getStringFromTable("meshPath", vector<Index>{Index(to_string(i + 1), false)});
					basePath[i] = luaManager->getStringFromTable("basePath", vector<Index>{Index(to_string(i + 1), false)});

					unitCornerPoints[i] = new Vector3[8];
					unitCuboidDimensions[i] = new Vector3[8];

					for(int j = 0; j < 8; j++){
							float cornerX = luaManager->getFloatFromTable("unitCornerPoints", vector<Index>{Index(to_string(i + 1), false), Index(to_string(j + 1), false), Index("x", true)});
							float cornerY = luaManager->getFloatFromTable("unitCornerPoints", vector<Index>{Index(to_string(i + 1), false), Index(to_string(j + 1), false), Index("y", true)});
							float cornerZ = luaManager->getFloatFromTable("unitCornerPoints", vector<Index>{Index(to_string(i + 1), false), Index(to_string(j + 1), false), Index("z", true)});
							unitCornerPoints[i][j] = Vector3(cornerX, cornerY, cornerZ);

							float dimX = luaManager->getFloatFromTable("unitCuboidDimensions", vector<Index>{Index(to_string(i + 1), false), Index(to_string(j + 1), false), Index("x", true)});
							float dimY = luaManager->getFloatFromTable("unitCuboidDimensions", vector<Index>{Index(to_string(i + 1), false), Index(to_string(j + 1), false), Index("y", true)});
							float dimZ = luaManager->getFloatFromTable("unitCuboidDimensions", vector<Index>{Index(to_string(i + 1), false), Index(to_string(j + 1), false), Index("z", true)});
							unitCuboidDimensions[i][j] = Vector3(dimX, dimY, dimZ);
					}
				}
		}
}
