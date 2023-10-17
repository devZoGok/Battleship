#include "transport.h"
#include "player.h"

namespace battleship{
	using namespace vb01;

	Transport::Transport(Player *player, int id, Vector3 pos, Quaternion rot) : Vehicle(player, id, pos, rot), Garrisonable(){}
}
