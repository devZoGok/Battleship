#ifndef ABSTRACT_APP_STATE
#define ABSTRACT_APP_STATE

#include "key.h"

#include <vector>
#include <util.h>

namespace battleship{
    enum class AppStateTypes {GUI_STATE,IN_GAME_STATE,ACTIVE_STATE};
    class AbstractAppState {
    public:
        AbstractAppState(){}
        ~AbstractAppState(){}
        void setKey(int, Mapping*);
        AppStateTypes getType() {return type;}
        inline bool isAttached(){return attached;}
        inline int getKeysNumber(){return attachedKeys.size();}
        inline Mapping* getKey(int i){return attachedKeys[i];}
        inline std::vector<Mapping*>& getKeys(){return attachedKeys;}
        virtual void onAttachment();
        virtual void onDetachment();
        virtual void update(){}
        virtual void onAction(Mapping::Bind, bool){}
        virtual void onAnalog(Mapping::Bind, double){}
        virtual void onRawKeyPress(vb01::u8){}
        virtual void onRawMousePress(vb01::u8){}
				virtual void onRawJoystickAxis(vb01::u8, float){}
				virtual void onRawJoystickKeyPress(vb01::u8){}
    private:
        bool attached = false;
        std::vector<Mapping*> attachedKeys;
    protected:
        AppStateTypes type;
        inline void attachKey(Mapping *k){attachedKeys.push_back(k);}
        void detachKey(Mapping*);
        void detachAllKeys();
    };
}

#endif
