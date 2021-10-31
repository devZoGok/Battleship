#pragma once
#ifndef CONSOLE_COMMAND_H
#define CONSOLE_COMMAND_H

#include <vector>

#include "unit.h"
#include "listbox.h"
#include "player.h"

namespace game
{
    namespace gui{
        class ConsoleCommand{
        public:
            ConsoleCommand(vb01Gui::Listbox*, std::vector<content::Player*>, std::string, std::vector<std::string>);
            ~ConsoleCommand();
            void execute();
        private:
						vb01Gui::Listbox *listbox;
            void printConsoleMessage(std::string);
            std::vector<content::Player*> players;
            std::string name;
            std::vector<std::string> arguments;
            std::string commandList[2] = {"addUnit","debugUnits"};
            // #0
            void addUnit();
            // #1
            void debugUnits();
        };
    }
}
#endif
