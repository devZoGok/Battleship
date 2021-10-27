#pragma once
#ifndef CONSOLE_COMMAND_H
#define CONSOLE_COMMAND_H

#include "gameManager.h"
#include "unit.h"
#include "listbox.h"
#include "player.h"
#include <irrlicht.h>
#include <vector>

namespace game
{
    namespace gui{
        class ConsoleCommand{
        public:
            ConsoleCommand(core::GameManager*,Listbox*,std::vector<content::Player*>,irr::core::stringw,std::vector<irr::core::stringw>);
            ~ConsoleCommand();
            void execute();
        private:
            core::GameManager *gameManager;
            Listbox *listbox;
            void printConsoleMessage(irr::core::stringw);
            std::vector<content::Player*> players;
            irr::core::stringw name;
            std::vector<stringw> arguments;
            irr::core::stringw commandList[2]={"addUnit","debugUnits"};
            // #0
            void addUnit();
            // #1
            void debugUnits();
        };
    }
}
#endif
