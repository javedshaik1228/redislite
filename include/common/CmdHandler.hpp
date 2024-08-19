#pragma once
#include <sstream>

namespace redislite
{
    namespace CmdHandler
    {
        std::string processCommand(const std::string &iCommand);

        std::string handleSetString(std::istringstream &iCmdStream);

        std::string handleGetString(std::istringstream &iCmdStream);
    }

}