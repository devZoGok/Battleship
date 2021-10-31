#include <root.h>

#include "map.h"
#include "gameManager.h"
#include "defConfigs.h"

using namespace game::core;
using namespace std;
using namespace vb01;

namespace game{
    namespace content{
        Map::Map() {
            size = Vector2(50, 50);
        }

        Map::~Map() {
        }

        void Map::update() {
        }

        void Map::load() {
						string path[]{
							PATH + "Textures/down.jpg",
							PATH + "Textures/left.jpg",
							PATH + "Textures/right.jpg",
							PATH + "Textures/front.jpg",
							PATH + "Textures/back.jpg"
						};

						Root::getSingleton()->createSkybox(path);

						/*
            waterMesh = smgr->addHillPlaneMesh("", dimension2d<float>(1, 1), dimension2d<u32>(30, 30), 0, .1, dimension2d<float>(2., 2.), dimension2d<float>(10, 10));
            waterNode = smgr->addWaterSurfaceSceneNode(waterMesh, .1, 900, 3);
            waterNode->setPosition(vector3df(0, 0, 0));
            waterNode->setMaterialTexture(0, driver->getTexture(PATH + "Textures/water-texture.png"));
            waterNode->setMaterialFlag(EMF_LIGHTING, true);
            waterNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
						*/
        }

        void Map::unload() {}
    }
}
