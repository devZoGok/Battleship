#include "eventListener.h"

using namespace irr;

namespace game{
    namespace core{
        EventListener::EventListener(GameManager* gM) {
            gameManager = gM;
        }

        EventListener::~EventListener() {}

        bool EventListener::OnEvent(const SEvent& event) {
            int offsetTrigger = -1;
            if (event.EventType == EET_KEY_INPUT_EVENT) {
                for (int i = 0; i < gameManager->getAppStateNumber(); i++) {
                    for (Key *k : gameManager->getAppState(i)->getKeys()) {
                        if (k->key && k->trigger == event.KeyInput.Key) {
                            if (!k->analog) {
                                if (!event.KeyInput.PressedDown)
                                    k->beingUsed=false;
                                if (!k->beingUsed) {
                                    if (event.KeyInput.PressedDown)
                                        k->beingUsed=true;
                                    k->pressed=event.KeyInput.PressedDown;
                                    gameManager->getAppState(i)->onAction(k->bind, k->pressed);
                                }
                            } else {
                                k->pressed=event.KeyInput.PressedDown;
                                gameManager->getAppState(i)->onAnalog(k->bind, 0.);
                            }
                        }
                        gameManager->getAppState(i)->onRawKeyPress(event.KeyInput);
                    }
                }
            }
            else if (event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event != EMIE_MOUSE_MOVED) {
                bool isPressed = false;
                switch (event.MouseInput.Event) {
                    case EMIE_LMOUSE_PRESSED_DOWN:
                        offsetTrigger = 0;
                        isPressed = true;
                        break;
                    case EMIE_LMOUSE_LEFT_UP:
                        offsetTrigger = 0;
                        isPressed = false;
                        break;
                    case EMIE_MMOUSE_PRESSED_DOWN:
                        offsetTrigger = 1;
                        isPressed = true;
                        break;
                    case EMIE_MMOUSE_LEFT_UP:
                        offsetTrigger = 1;
                        isPressed = false;
                        break;
                    case EMIE_RMOUSE_PRESSED_DOWN:
                        offsetTrigger = 2;
                        isPressed = true;
                        break;
                    case EMIE_RMOUSE_LEFT_UP:
                        offsetTrigger = 2;
                        isPressed = false;
                        break;
                    case EMIE_MOUSE_WHEEL:
                        if (event.MouseInput.Wheel == 1)
                            offsetTrigger = 3;
                        else
                            offsetTrigger = 4;
                        break;
                }
                for (int i = 0; i < gameManager->getAppStateNumber(); i++) {
                    for (Key *k : gameManager->getAppState(i)->getKeys()) {
                        if (offsetTrigger == k->trigger) {
                            if (!k->analog) {
                                k->pressed=isPressed;
                                gameManager->getAppState(i)->onAction(k->bind, k->pressed);
                            } else {
                                k->beingUsed=isPressed;
                            }
                        }
                        if(offsetTrigger<=2)
                            gameManager->getAppState(i)->onRawMousePress(event.MouseInput);
                    }
                }
            }
            return false;
        }

        void EventListener::update() {
            for (int i = 0; i < gameManager->getAppStateNumber(); i++) {
                for (Key *k : gameManager->getAppState(i)->getKeys()) {
                    if (k->beingUsed && k->analog) {
                        gameManager->getAppState(i)->onAnalog(k->bind, 0.);
                    }
                }
            }
        }
    }
}
