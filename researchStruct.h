#ifndef RESEARCH_STRUCT_H
#define RESEARCH_STRUCT_H

#include "structure.h"

namespace battleship{
	class ResearchStruct : public Structure{
		public:
			ResearchStruct(Player*, int, vb01::Vector3, vb01::Quaternion, int = 0, Unit::State = Unit::State::STAND_GROUND);
			void update();
		private:
			vb01::s64 lastGenTime = 0;
			int researchCost, generationRate, generationSpeed;

			bool canGenerateResearch(){return vb01::getTime() - lastGenTime > generationRate;}
			void generateResearch();
			void researchTechnology(int);
	};
}

#endif
