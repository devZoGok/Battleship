#ifndef VEHICLE_H
#define VEHICLE_H

#include "unit.h"

namespace vb01{
	class Material;
}

namespace battleship{
	class Garrisonable;

	class Vehicle : public Unit{
		public:
			Vehicle(Player*, int, vb01::Vector3, vb01::Quaternion);
			~Vehicle();
			virtual void update();
        	void move(Order);
			void exitGarrisonable();
			inline Unit* getGarrisonable(){return garrisonable;}
		private:
		   	Unit *garrisonable = nullptr;
			bool pursuingTarget = false;
			int patrolPointId = 0;
        	float speed, maxTurnAngle, anglePrecision;
			vb01::Material *debugMat = nullptr;
			std::vector<vb01::Node*> debugPathPoints;

        	inline int getNextPatrolPointId(int numPoints) {return patrolPointId == numPoints - 1 ? 0 : patrolPointId + 1;}
			void enterGarrisonable();
			void navigateToMovingTarget(float);
			void halt();
			void turn(float);
			void addOrder(Order);
			void advance(float, MoveDir = MoveDir::FORW);
			void preparePathpoints(Order);
			void removePathpoint(int = 0);
			void removeAllPathpoints();
			void toggleSelection(bool);
		protected:
			std::vector<vb01::Vector3> pathPoints;

        	void navigate(float = 0.);
			void alignToSurface();
			void attack(Order);
			void garrison(Order);
			virtual void initProperties();
			virtual void fire();
	};
}

#endif
