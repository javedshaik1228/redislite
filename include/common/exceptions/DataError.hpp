//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdexcept>
#include <string>

namespace redislite
{
class DataError : public std::runtime_error
{
   public:
	explicit DataError(const std::string &message) : std::runtime_error(message) {}
};
}  // namespace redislite
