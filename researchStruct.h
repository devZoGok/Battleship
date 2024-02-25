#ifndef RESEARCH_STRUCT_H
#define RESEARCH_STRUCT_H

#include "structure.h"

namespace battleship{
	class ResearchStruct : public Structure{
		public:
			ResearchStruct(Player*, int, vb01::Vector3, vb01::Quaternion, int = 0, Unit::State = Unit::State::STAND_GROUND);
			void update();
			inline void appendToQueue(int tid){researchQueue.push_back(tid);}
		private:
			vb01::s64 lastUpdateTime = 0;
			int researchCost, generationRate, generationSpeed, researchStatus = 0;
			std::vector<int> researchQueue;

			bool canUpdateResearch(){return vb01::getTime() - lastGenTime > generationRate;}
			void generateResearch();
	};
}

#endif
