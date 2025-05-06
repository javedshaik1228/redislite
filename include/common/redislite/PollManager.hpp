//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <unordered_map>
#include <poll.h>

namespace redislite
{

class PollManager
{
   public:
	void addFd(int fd, short events);
	void updateEvents(int fd, short events);
	void removeFd(int fd);
	int wait(int timeoutMs);  // timeout in milliseconds
	std::vector<int> getReadyFds(short eventMask) const;

   private:
	std::vector<pollfd> _pollFds;
	// fd -> index from _pollFds map
	std::unordered_map<int, std::size_t> _fdIndexMap;
};

}  // namespace redislite
