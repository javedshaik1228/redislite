//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <sstream>

namespace redislite::CmdHandler
{
std::string processCommand(const std::string &iCommand);

std::string handleSetString(std::istringstream &iCmdStream);

std::string handleGetString(std::istringstream &iCmdStream);
}  // namespace redislite::CmdHandler