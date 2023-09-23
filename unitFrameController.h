#ifndef UNIT_FRAME_CONTROLLER_H
#define UNIT_FRAME_CONTROLLER_H

#include <vector>
#include <string>

#include <vector.h>
#include <quaternion.h>

namespace vb01{
	class Model;
}

namespace battleship{
	class UnitFrameController{
		public:
			struct UnitFrame{
				enum Status{PLACEABLE, NOT_PLACEABLE, PLACED};
			
				UnitFrame(std::string, int, int, vb01::Vector3 = vb01::Vector3::VEC_ZERO, vb01::Quaternion = vb01::Quaternion::QUAT_W);
				~UnitFrame(){}
				void update();
			
				vb01::Model *model;
				int id, type;
				Status status;
			};

			static UnitFrameController* getSingleton();
			void update();
			void removeUnitFrame(int);
			void removeUnitFrames();
			void rotateUnitFrames(float);
			inline int getNumUnitFrames(){return unitFrames.size();}
			inline void addUnitFrame(UnitFrame u){unitFrames.push_back(u);}
			inline UnitFrame& getUnitFrame(int i){return unitFrames[i];}
			inline bool isPlacingFrames(){return placingStructures;}
			inline void setPlacingFrames(bool ps){placingStructures = ps;}
			inline bool isPaintSelecting(){return paintSelecting;}
			inline void setPaintSelecting(bool ps){paintSelecting = ps;}
			inline bool isRotatingFrames(){return rotatingStructure;}
			inline void setRotatingFrames(bool rs){rotatingStructure = rs;}
			inline void setPaintSelectRowStart(vb01::Vector3 st){paintSelectRowStart = st;}
		private:
			UnitFrameController(){}
			void paintSelect(vb01::Vector3, float, float);
			void placeUnitFrame(int, vb01::Vector3, float, float);

			std::vector<UnitFrame> unitFrames;
			vb01::Vector3 paintSelectRowStart, rowDir;
	   		bool placingStructures = false, paintSelecting = false, rotatingStructure = false;
	};
}

#endif
