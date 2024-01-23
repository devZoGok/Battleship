#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "structure.h"

namespace battleship{
	class Extractor : public Structure{
		public:
			Extractor(Player*, int, vb01::Vector3, vb01::Quaternion, int);
		private:
	};
}

#endif
