#include "submarine.h"
#include "player.h"
#include "gameObjectFactory.h"

#include <solUtil.h>

namespace battleship{
	using namespace vb01;
	using namespace gameBase;

	Submarine::Submarine(Player *player, int id, Vector3 pos, Quaternion rot) : Vehicle(player, id, pos, rot){}

	void Submarine::fire(){
		int projId = 1;
		sol::table SOL_STATE_VIEW = generateView()["projectiles"];
		sol::table posTable = SOL_STATE_VIEW["pos"][projId + 1], rotTable = SOL_STATE_VIEW["rot"][projId + 1];
		Vector3 projPos = Vector3(posTable["x"], posTable["y"], posTable["z"]);
		projPos = pos + leftVec * projPos.x + upVec * projPos.y + dirVec * projPos.z;
		Quaternion projRot = Quaternion(rotTable["w"], rotTable["x"], rotTable["y"], rotTable["z"]) * rot;

		player->addProjectile(GameObjectFactory::createProjectile(this, projId, projPos, projRot));
		Unit::fire();
	}
}
