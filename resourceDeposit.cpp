#include "resourceDeposit.h"

namespace battleship{
	using namespace vb01;

	ResourceDeposit::ResourceDeposit(Player *player, int id, Vector3 pos, Quaternion rot) : GameObject(GameObject::Type::RESOURCE_DEPOSIT, id, player, pos, rot){
		initModel();
		placeAt(pos);
		orientAt(rot);
	}

	void ResourceDeposit::update(){
	}
}
