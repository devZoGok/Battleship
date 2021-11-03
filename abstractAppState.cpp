#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <util.h>

#include "abstractAppState.h"
#include "defConfigs.h"

using namespace vb01;
using namespace std;

namespace battleship{
	using namespace configData;

    void AbstractAppState::onAttachment() {
        const int stateId = (int)type, numBinds = configData::numBinds[stateId];
        int firstLine=0,lastLine;

        for(int i=0;i<stateId;i++)
            firstLine += configData::numConfBinds[i];

           //firstLine++;
        lastLine=firstLine + configData::numConfBinds[stateId];
        std::vector<string> lines;
        readFile(PATH + "../options.cfg", lines, firstLine,lastLine);

        for(int i=0;i<numBinds;i++){
            int trigger=triggers[stateId][i];

            if(i<configData::numConfBinds[stateId]&&stateId!=(int)AppStateTypes::GUI_STATE){
                int xId=-1;

                for(int i2=0;i2<lines[i].size()&&xId==-1;i2++)
                    if(lines[i].c_str()[i2]=='x')
                        xId=i2;

                char ch[2];

                for(int i2=0;i2<2;i2++)
                    ch[i2]=(char)lines[i].substr(xId+1,2).c_str()[i2];

                stringstream ss;
                ss<<ch;
                ss>>std::hex>>trigger;
            }

						Mapping::Bind bind=binds[stateId][i];
						Mapping::BindType type = (stateId > 0 ? Mapping::MOUSE_KEY : Mapping::KEYBOARD);
						bool isAnalog = configData::isAnalog[stateId][i];

						Mapping *m = new Mapping;
						m->bind = bind;
						m->trigger = trigger;
						m->type = type;
						m->action = !isAnalog;
            attachedKeys.push_back(m);
        }

        attached = true;
    }

    void AbstractAppState::onDetachment() {
        attached = false;
    }

    void AbstractAppState::detachKey(Mapping *key) {
        for (int i = 0; i < attachedKeys.size(); i++)
            if (key == attachedKeys[i]) {
                delete key;
                attachedKeys.erase(attachedKeys.begin() + i);
            }
    }

    void AbstractAppState::detachAllKeys() {
        while (attachedKeys.size() > 0) {
            delete attachedKeys[attachedKeys.size() - 1];
            attachedKeys.pop_back();
        }
    }
}
