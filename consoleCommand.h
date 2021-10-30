#pragma once
#ifndef CONSOLE_COMMAND_H
#define CONSOLE_COMMAND_H

#include <irrlicht.h>
#include <vector>

#include "gameManager.h"
#include "unit.h"
#include "listbox.h"
#include "player.h"

namespace game
{
    namespace gui{
        class ConsoleCommand{
        public:
            ConsoleCommand(Listbox*,std::vector<content::Player*>,irr::core::stringw,std::vector<irr::core::stringw>);
            ~ConsoleCommand();
            void execute();
        private:
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
