#ifndef FACTORY_H
#define FACTORY_H

#include "structure.h"

#include <vector>
#include <util.h>

namespace vb01{
	class Node;
}

namespace battleship{
	class Player;

	class Factory : public Structure{
		public:
			Factory(Player*, int, vb01::Vector3, vb01::Quaternion, int = 0);
			~Factory(){}
			void update();
			void appendToQueue(int);
			inline std::vector<int> getQueue(){return unitQueue;}
		private:
			const int MAX_LEN_QUEUE = 10;
			int trainingStatus = 0, rateOfTraining = 50;
			vb01::s64 lastTrainTime = 0;
			std::vector<int> unitQueue;

			void train();
			void initProperties();
			inline bool canTrain(){return vb01::getTime() - lastTrainTime > rateOfTraining;}
	};
}

#endif
