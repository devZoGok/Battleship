#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "structure.h"

#include <util.h>

namespace battleship{
	class ResourceDeposit;

	class Extractor : public Structure{
		public:
			Extractor(Player*, int, vb01::Vector3, vb01::Quaternion, int, ResourceDeposit *rd = nullptr);
			~Extractor();
			void update();
			void draw();
		private:
			int drawRate;
			vb01::s64 lastDrawTime = 0;
			vb01::Node *ammountBackground = nullptr, *ammountForeground = nullptr;
			ResourceDeposit *deposit = nullptr;

			bool canDraw();
	};
}

#endif
