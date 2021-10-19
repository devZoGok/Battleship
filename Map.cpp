#include "Map.h"
#include "DefConfigs.h"

using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;
using namespace irr::video;
using namespace game::core;

namespace game{
    namespace content{
        Map::Map(GameManager *gM) {
            gameManager = gM;
            size = vector2d<s32>(50, 50);
        }

        Map::~Map() {
        }

        void Map::update() {
        }

        void Map::load() {
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            smgr->setAmbientLight(SColor(255, 100, 100, 100));
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
//             sunLight=smgr->addLightSceneNode(0,vector3df(0,20,0),SColor(255,255,255,255));
//             sunLight->setLightType(E_LIGHT_TYPE::ELT_COUNT);
            ISceneNode *skybox = smgr->addSkyBoxSceneNode(driver->getTexture(PATH + "Textures/up.jpg")
                    , driver->getTexture(PATH + "Textures/down.jpg")
                    , driver->getTexture(PATH + "Textures/left.jpg")
                    , driver->getTexture(PATH + "Textures/right.jpg")
                    , driver->getTexture(PATH + "Textures/front.jpg")
                    , driver->getTexture(PATH + "Textures/back.jpg"));
            waterMesh = smgr->addHillPlaneMesh("", dimension2d<float>(1, 1), dimension2d<u32>(30, 30), 0, .1, dimension2d<float>(2., 2.), dimension2d<float>(10, 10));
            waterNode = smgr->addWaterSurfaceSceneNode(waterMesh, .1, 900, 3);
            waterNode->setPosition(vector3df(0, 0, 0));
            waterNode->setMaterialTexture(0, driver->getTexture(PATH + "Textures/water-texture.png"));
            waterNode->setMaterialFlag(EMF_LIGHTING, true);
            waterNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
        }

        void Map::unload() {}
    }
}
