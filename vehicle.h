#ifndef VEHICLE_H
#define VEHICLE_H

#include "unit.h"

namespace battleship{
	class Vehicle : public Unit{
		public:
			Vehicle(Player*, int, vb01::Vector3, vb01::Quaternion);
        	void move(Order);
		private:
			int patrolPointId = 0;
        	float speed, maxTurnAngle, anglePrecision;

        	inline int getNextPatrolPointId(int numPoints) {return patrolPointId == numPoints - 1 ? 0 : patrolPointId + 1;}
			void halt();
			void turn(float);
			void addOrder(Order order);
			void initProperties();
			void advance(float, MoveDir = MoveDir::FORW);
			void preparePathpoints(Order);
		protected:
			std::vector<vb01::Vector3> pathPoints;

        	void navigate(Order, float = 0.);
			void alignToSurface();
	};
}

#endif
