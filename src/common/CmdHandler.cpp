#include <CmdHandler.hpp>

#include <CmdConsts.hpp>
#include <utils/LoggerMacros.hpp>
#include <exceptions/DataError.hpp>

#include <data/StringStore.hpp>

namespace redislite
{
    namespace CmdHandler
    {
        std::string processCommand(const std::string &iCommand)
        {
            try
            {
                std::istringstream iss(iCommand);
                std::string cmd;
                iss >> cmd;

                if (cmd == CmdConsts::kSETCmd)
                {
                    return handleSetString(iss);
                }
                if (cmd == redislite::CmdConsts::kGETCmd)
                {
                    return handleGetString(iss);
                }
                else
                {
                    return "-ERR unknown command \r\n";
                }
            }
            catch (redislite::DataError &de)
            {
                LOG_CSL_ERR("Data error: " + std::string(de.what()));
                return "$-1\r\n";
            }
        }

        std::string handleSetString(std::istringstream &iCmdStream)
        {
            std::string aKey, aValue;
            iCmdStream >> aKey >> aValue;
            StringStore::getInstance()->put(aKey, aValue);
            return "+OK\r\n";
        }

        std::string handleGetString(std::istringstream &iCmdStream)
        {
            std::string aKey, oValue;
            iCmdStream >> aKey;
            oValue = StringStore::getInstance()->get(aKey);
            std::string response = "$" + std::to_string(oValue.size()) + "\r\n" + oValue + "\r\n";
            return response;
        }
    }

}