//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace redislite
{
class InternalError : public std::runtime_error
{
   public:
	explicit InternalError(const std::string &message) : std::runtime_error(message) {}

	explicit InternalError(const std::stringstream &message) : std::runtime_error(message.str()) {}

	explicit InternalError(const char *message) : std::runtime_error(message) {}
};
}  // namespace redislite
