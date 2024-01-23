#include "resourceRover.h"

namespace battleship{
	using namespace std;
	using namespace vb01;

	ResourceRover::ResourceRover(Player *player, int id, Vector3 pos, Quaternion rot) : Vehicle(player, id, pos, rot){}
}
