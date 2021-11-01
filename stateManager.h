#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <vector>

#include "abstractAppState.h"

namespace battleship{
		class StateManager{
				public:
						StateManager(){}
						void update();
						void attachState(AbstractAppState*);
        		void dettachState(AbstractAppState*);
        		void dettachState(AppStateTypes);
						AbstractAppState* getAppState(AppStateTypes);
        		inline AbstractAppState* getAppState(int id){return appStates[id];}
        		inline void setAppState(int i, AbstractAppState *a){appStates[i]=a;}
        		inline int getAppStateNumber(){return appStates.size();}
						inline std::vector<AbstractAppState*> getAppStates(){return appStates;}
				private:
       		std::vector<AbstractAppState*> appStates;
		};
}

#endif
