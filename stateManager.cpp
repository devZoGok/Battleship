#include "stateManager.h"
#include "abstractAppState.h"

namespace battleship{
    void StateManager::attachState(AbstractAppState *a) {
        bool alreadyAttached = false;
        for (int i = 0; i < appStates.size()&&!alreadyAttached; i++)
            if (appStates[i] == a)
                alreadyAttached = true;
        if (!alreadyAttached) {
            a->onAttachment();
            appStates.push_back(a);
        }
    }

    void StateManager::dettachState(AbstractAppState *a) {
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

    void StateManager::update() {
        for (AbstractAppState *a : appStates)
            if (a->isAttached() && a)
                a->update();
    }

    AbstractAppState* StateManager::getAppState(AppStateTypes t) {
        AbstractAppState *state = nullptr;
        for (AbstractAppState *a : appStates)
            if (a->getType() == t)
                state = a;
        return state;
    }
}
