#ifndef UNIT_FRAME_CONTROLLER_H
#define UNIT_FRAME_CONTROLLER_H

#include <vector>
#include <string>

namespace vb01{
	class Model;
}

namespace battleship{
	class UnitFrameController{
		public:
			struct UnitFrame{
				enum Status{PLACEABLE, NOT_PLACEABLE, PLACED};
			
				UnitFrame(std::string, int, int);
				~UnitFrame(){}
				void update();
			
				vb01::Model *model;
				int id, type;
				Status status;
			};

			static UnitFrameController* getSingleton();
			void update();
			void removeUnitFrames();
			void rotateUnitFrames(float);
			inline void addUnitFrame(UnitFrame u){unitFrames.push_back(u);}
			inline UnitFrame& getUnitFrame(int i){return unitFrames[i];}
			inline bool isPlacingFrames(){return placingStructures;}
			inline void setPlacingFrames(bool ps){placingStructures = ps;}
			inline bool isRotatingFrames(){return rotatingStructure;}
			inline void setRotatingFrames(bool rs){rotatingStructure = rs;}
		private:
			UnitFrameController(){}

			std::vector<UnitFrame> unitFrames;
	   		bool placingStructures = false, rotatingStructure = false;
	};
}

#endif
