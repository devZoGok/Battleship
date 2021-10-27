#include "abstractAppState.h"
#include "defConfigs.h"
#include "util.h"
#include <irrlicht.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace irr::core;
using namespace game::util;
using namespace game::core;
using namespace std;

namespace game{
    namespace core{
        void AbstractAppState::onAttachment() {
            const int stateId=(int)type,numBinds=core::numBinds[stateId];
            int firstLine=0,lastLine;
            for(int i=0;i<stateId;i++)
                firstLine+=core::numConfBinds[i];
//             firstLine++;
            lastLine=firstLine+core::numConfBinds[stateId];
            std::vector<stringw> lines=readFile(PATH_STR+"../options.cfg",firstLine,lastLine);
            for(int i=0;i<numBinds;i++){
                int trigger=core::triggers[stateId][i];
                if(i<core::numConfBinds[stateId]&&stateId!=(int)AppStateTypes::GUI_STATE){
                    int xId=-1;
                    for(int i2=0;i2<lines[i].size()&&xId==-1;i2++)
                        if(lines[i].c_str()[i2]=='x')
                            xId=i2;
                    char ch[2];
                    for(int i2=0;i2<2;i2++)
                        ch[i2]=(char)lines[i].subString(xId+1,2).c_str()[i2];
                    stringstream ss;
                    ss<<ch;
                    ss>>std::hex>>trigger;
                }
                Bind bind=binds[stateId][i];
                bool isKey=stateId>0?trigger>4:core::triggers[stateId][i],isAnalog=core::isAnalog[stateId][i];
                attachedKeys.push_back(new Key(bind,trigger,isKey,isAnalog));
            }
            attached = true;
        }

        void AbstractAppState::onDetachment() {
            attached = false;
        }

        void AbstractAppState::detachKey(Key *key) {
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
}
