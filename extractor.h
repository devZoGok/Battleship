#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "structure.h"

#include <util.h>

namespace battleship{
	class Extractor : public Structure{
		public:
			Extractor(Player*, int, vb01::Vector3, vb01::Quaternion, int);
		private:
			int drawRate;
			vb01::s64 lastDrawTime = 0;
	};
}

#endif
