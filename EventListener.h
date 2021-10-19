#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include"GameManager.h"

namespace game{
    namespace core{
        class EventListener : public irr::IEventReceiver {
        public:
            EventListener(GameManager*);
            ~EventListener();
            virtual bool OnEvent(const irr::SEvent&);
            void update();
        private:
            irr::SEvent* event;
            bool keyEvent = false, mouseEvent = false;
            GameManager* gameManager;
        };
    }
}

#endif
