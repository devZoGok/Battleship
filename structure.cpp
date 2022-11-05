#include "structure.h"

using namespace gameBase;
using namespace vb01;
using namespace std;

namespace battleship{
	Structure::Structure(Player *player, int id, Vector3 pos, Quaternion rot) : Unit(player, id, pos, rot){
	}
}
