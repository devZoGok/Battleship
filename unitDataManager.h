#ifndef UNIT_DATA_MANAGER_H
#define UNIT_DATA_MANAGER_H

#include <string>

#include <vector.h>

namespace battleship{
    enum class UnitType {VESSEL, DESTROYER, CRUISER, AIRCRAFT_CARRIER, SUBMARINE, MISSILE_JET, DEMO_JET};

	class UnitDataManager{
			public:
				static UnitDataManager* getSingleton();
				inline int getNumUnits(){return numUnits;}
				inline int* getHealth(){return health;}
				inline UnitType* getUnitType(){return unitType;}
				inline float* getMaxTurnAngle(){return maxTurnAngle;}
				inline float* getRange(){return range;}
				inline float* getLineOfSight(){return lineOfSight;}
				inline float* getSpeed(){return speed;}
				inline float* getAnglePrecision(){return anglePrecision;}
				inline float* getDestinationOffset(){return destinationOffset;}
				inline vb01::Vector3** getUnitCornerPoints(){return unitCornerPoints;}
				inline std::string* getName(){return name;}
				inline std::string* getBasePath(){return basePath;}
				inline std::string* getMeshPath(){return meshPath;}
			private:
				UnitDataManager();

			int numUnits;
    		UnitType *unitType;
    		int *health;
    		float *speed;
    		float *destinationOffset;
    		float *anglePrecision;
    		int *cost;
    		float *maxTurnAngle;
    		float *range;
    		vb01::Vector3 **unitCornerPoints;
    		vb01::Vector3 **unitCuboidDimensions;
    		float *unitAxisLength;
    		float *lineOfSight;
    		std::string *name;
    		std::string *meshPath;
    		std::string *basePath;
    };
}

#endif
