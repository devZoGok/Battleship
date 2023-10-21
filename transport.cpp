#include "transport.h"
#include "player.h"

#include <solUtil.h>

namespace battleship{
	using namespace vb01;
	using namespace gameBase;

	Transport::Transport(Player *player, int id, Vector3 pos, Quaternion rot) : Vehicle(player, id, pos, rot){

	}
}
