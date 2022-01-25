#ifndef UNIT_DATA_MANAGER_H
#define UNIT_DATA_MANAGER_H

#include <string>

#include <vector.h>

namespace battleship{
		class UnitDataManager{
				public:
						static UnitDataManager* getSingleton();
				private:
						UnitDataManager();

        		enum class UnitType {VESSEL, DESTROYER, CRUISER, AIRCRAFT_CARRIER, SUBMARINE, MISSILE_JET, DEMO_JET};
						int numUnits;
        		UnitType unitType;
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
