#include <algorithm>
#include <irrlicht.h>

#include "gameManager.h"
#include "guiAppState.h"
#include "stateManager.h"
#include "eventListener.h"
#include "abstractBitmapText.h"
#include "abstractImage.h"

using namespace std;
using namespace game::gui;
using namespace irr;

namespace game{
    namespace core{
				GameManager *gameManager = nullptr;

				GameManager* GameManager::getSingleton(){
						if(!gameManager)
								gameManager = new GameManager();

						return gameManager;
				}

        GameManager::GameManager() {
    				device = createDevice(video::EDT_OPENGL, irr::core::dimension2d<u32>(800,600), 32, false, true, true, nullptr);
    				device->setResizable(true);
						irr::video::IVideoDriver *driver = device->getVideoDriver();
						irr::scene::ISceneManager *smgr = device->getSceneManager();
						irr::gui::IGUIEnvironment *guiEnv = device->getGUIEnvironment();

    				device->setWindowCaption(L"(((A)))");
						stateManager = new StateManager();
            listener = new EventListener(this);
            device->setEventReceiver(listener);
            width = device->getVideoDriver()->getScreenSize().Width;
            height = device->getVideoDriver()->getScreenSize().Height;
        }

        GameManager::~GameManager() {}

        void GameManager::detachBitmapText(AbstractBitmapText *bitmapText) {
            for (int i = 0; i < bitmapTexts.size(); i++)
                if (bitmapText == bitmapTexts[i]) {
                    delete bitmapText;
                    bitmapTexts.erase(bitmapTexts.begin() + i);
                }
        }

        void GameManager::detachAllBitmapTexts() {
            while (bitmapTexts.size() > 0) {
                delete bitmapTexts[bitmapTexts.size() - 1];
                bitmapTexts.pop_back();
            }
        }

        void GameManager::detachImage(AbstractImage *image) {
            for (int i = 0; i < images.size(); i++)
                if (image == images[i]) {
                    delete image;
                    images.erase(images.begin() + i);
                }
        }

        void GameManager::detachAllImages() {
            while (images.size() > 0) {
                delete images[images.size() - 1];
                images.pop_back();
            }
        }

        void GameManager::update() {
            listener->update();
						stateManager->update();

            for (BitmapText *b : bitmapTexts)
                b->update();

            for (Image *i : images)
                i->update();
        }
    }
}
