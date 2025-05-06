//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <redislite/PollManager.hpp>
#include <algorithm>

namespace redislite
{

void PollManager::addFd(int fdToAdd, short events)
{
	if (_fdIndexMap.count(fdToAdd)) return;	 // already tracked

	pollfd pfd{fdToAdd, events, 0};
	_pollFds.emplace_back(pfd);
	_fdIndexMap[fdToAdd] = _pollFds.size() - 1;
}

void PollManager::updateEvents(int fdToUpdate, short events)
{
	if (_fdIndexMap.count(fdToUpdate))
	{
		_pollFds[_fdIndexMap[fdToUpdate]].events = events;
	}
}

void PollManager::removeFd(int fdToRemove)
{
	auto fdItr = _fdIndexMap.find(fdToRemove);
	if (fdItr == _fdIndexMap.end())
	{
		return;
	}

	size_t idx = fdItr->second;
	// move to back for easy vector removal with pop_back
	std::swap(_pollFds[idx], _pollFds.back());
	_fdIndexMap[_pollFds[idx].fd] = idx;

	_pollFds.pop_back();
	_fdIndexMap.erase(fdToRemove);
}

int PollManager::wait(int timeoutMs)
{
	int result = poll(_pollFds.data(), _pollFds.size(), timeoutMs);
	return result;
}

std::vector<int> PollManager::getReadyFds(short eventMask) const
{
	std::vector<int> readyFds;
	for (const auto& itrPollFd : _pollFds)
	{
		if (itrPollFd.revents & eventMask)
		{
			readyFds.emplace_back(itrPollFd.fd);
		}
	}
	return readyFds;
}

}  // namespace redislite
