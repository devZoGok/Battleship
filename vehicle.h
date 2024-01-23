#ifndef VEHICLE_H
#define VEHICLE_H

#include "unit.h"

namespace vb01{
	class Material;
}

namespace battleship{
	class Vehicle : public Unit{
		public:
			Vehicle(Player*, int, vb01::Vector3, vb01::Quaternion);
			~Vehicle();
			virtual void update();
        	void move(Order);
			void exitGarrisonable(vb01::Vector3);
			inline Unit* getGarrisonable(){return garrisonable;}
			inline int getGarrisonCategory(){return garrisonCategory;}
		private:
		   	Unit *garrisonable = nullptr;
			bool pursuingTarget = false;
			int patrolPointId = 0, garrisonCategory;
        	float speed, maxTurnAngle, anglePrecision;
			vb01::Material *debugMat = nullptr;
			std::vector<vb01::Node*> debugPathPoints;

        	inline int getNextPatrolPointId(int numPoints) {return patrolPointId == numPoints - 1 ? 0 : patrolPointId + 1;}
			void enterGarrisonable();
			void navigateToTarget(float);
			void halt();
			void turn(float);
			void addOrder(Order);
			void advance(float, MoveDir = MoveDir::FORW);
			void addPathpoint(vb01::Vector3);
			void removePathpoint(int = 0);
			void removeAllPathpoints();
			void select();
		protected:
			std::vector<vb01::Vector3> pathPoints;

        	void navigate(float = 0.);
			void preparePathpoints(Order&, vb01::Vector3);
			void alignToSurface();
			void attack(Order);
			void garrison(Order);
			void patrol(Order);
			virtual void initProperties();
	};
}

#endif
