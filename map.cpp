#include <root.h>
#include <quad.h>
#include <node.h>
#include <material.h>

#include "map.h"
#include "gameManager.h"
#include "defConfigs.h"

using namespace std;
using namespace vb01;

namespace battleship{
	using namespace configData;

    Map::Map() {
        size = Vector2(50, 50);
    }

    Map::~Map() {
    }

    void Map::update() {
    }

    void Map::load() {
				string path[]{
					PATH + "Textures/left.jpg",
					PATH + "Textures/right.jpg",
					PATH + "Textures/top.jpg",
					PATH + "Textures/bottom.jpg",
					PATH + "Textures/front.jpg",
					PATH + "Textures/back.jpg"
				};

				Root *root = Root::getSingleton();
				root->createSkybox(path);

				Quad *quad = new Quad(Vector3(size.x, size.y, 1) * 1, true);
				Material *mat = new Material(root->getLibPath() + "texture");
				mat->addBoolUniform("texturingEnabled", true);
				mat->addBoolUniform("lightingEnabled", false);
				string fr[]{PATH + "Textures/water.jpg"};
				Texture *t = new Texture(fr, 1, false);
				mat->addTexUniform("textures[0]", t, true);
				quad->setMaterial(mat);
				Node *water = new Node();
				water->attachMesh(quad);
				root->getRootNode()->attachChild(water);
				water->setOrientation(Quaternion(-1.57, Vector3::VEC_I));

				Camera *cam = root->getCamera();
				cam->setPosition(Vector3(1, 1, 1) * 10);
				cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
    }

    void Map::unload() {}
}
