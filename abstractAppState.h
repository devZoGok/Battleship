#ifndef ABSTRACT_APP_STATE
#define ABSTRACT_APP_STATE

#include "key.h"
#include <vector>
#include <IEventReceiver.h>

namespace game{
    namespace core{
        enum class AppStateTypes {GUI_STATE,IN_GAME_STATE,ACTIVE_STATE};
        class AbstractAppState {
        public:
            AbstractAppState(){}
            ~AbstractAppState(){}
            void setKey(int, Key*);
            AppStateTypes getType() {return type;}
            inline bool isAttached(){return attached;}
            inline int getKeysNumber(){return attachedKeys.size();}
            inline Key* getKey(int i){return attachedKeys[i];}
            inline std::vector<Key*>& getKeys(){return attachedKeys;}
            virtual void onAttachment();
            virtual void onDetachment();
            virtual void update(){}
            virtual void onAction(Bind, bool){}
            virtual void onAnalog(Bind, double){}
            virtual void onRawKeyPress(irr::SEvent::SKeyInput){}
            virtual void onRawMousePress(irr::SEvent::SMouseInput){}
        private:
            bool attached = false;
            std::vector<Key*> attachedKeys;
        protected:
            AppStateTypes type;
            inline void attachKey(Key *k){attachedKeys.push_back(k);}
            void detachKey(Key*);
            void detachAllKeys();
        };
    }
}

#endif
