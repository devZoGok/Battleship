#include <algorithm>

#include "gameManager.h"
#include "guiAppState.h"
#include "eventListener.h"
#include "abstractBitmapText.h"
#include "abstractImage.h"

using namespace std;
using namespace game::gui;
using namespace irr;

namespace game{
    namespace core{
        GameManager::GameManager(IrrlichtDevice *dev) {
            device = dev;
            listener = new EventListener(this);
            device->setEventReceiver(listener);
            width = device->getVideoDriver()->getScreenSize().Width;
            height = device->getVideoDriver()->getScreenSize().Height;
        }

        GameManager::~GameManager() {}

        void GameManager::attachState(AbstractAppState *a) {
            bool alreadyAttached = false;
            for (int i = 0; i < appStates.size()&&!alreadyAttached; i++)
                if (appStates[i] == a)
                    alreadyAttached = true;
            if (!alreadyAttached) {
                a->onAttachment();
                appStates.push_back(a);
            }
        }

        void GameManager::dettachState(AbstractAppState *a) {
            bool alreadyDetached = false;
            for (int i = 0; i < appStates.size()&&!alreadyDetached; i++)
                if (appStates[i] == a)
                    alreadyDetached = true;
            if (!alreadyDetached) {
                a->onDetachment();
                for (int i = 0; i < appStates.size(); i++)
                    if (a == appStates[i]) {
                        appStates.erase(appStates.begin() + i);
                    }
            }
        }

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
            for (AbstractAppState *a : appStates)
                if (a->isAttached() && a)
                    a->update();
            for (BitmapText *b : bitmapTexts)
                b->update();
            for (Image *i : images)
                i->update();
        }

        AbstractAppState* GameManager::getAppState(AppStateTypes t) {
            AbstractAppState *state = nullptr;
            for (AbstractAppState *a : appStates)
                if (a->getType() == t)
                    state = a;
            return state;
        }
    }
}
