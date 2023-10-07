#include "resourceDeposit.h"

namespace battleship{
	using namespace vb01;

	ResourceDeposit::ResourceDeposit(int id, Vector3 pos, Quaternion rot) : GameObject(GameObject::Type::RESOURCE_DEPOSIT, id, nullptr, pos, rot){
		initModel();
		placeAt(pos);
		orientAt(rot);
	}

	void ResourceDeposit::update(){
	}
}
