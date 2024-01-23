#include "extractor.h"

namespace battleship{
	using namespace std;
	using namespace vb01;

	Extractor::Extractor(Player *player, int id, Vector3 pos, Quaternion rot, int buildStatus) : Structure(player, id, pos, rot, buildStatus){}
}
