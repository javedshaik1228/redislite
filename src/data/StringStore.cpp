#include <data/StringStore.hpp>
#include <exceptions/DataError.hpp>

namespace redislite
{
    std::unordered_map<std::string, std::string> StringStore::_stringMap;

    StringStore::StringStore()
    {
    }

    void StringStore::put(const std::string &iKey, const std::string &iValue)
    {
        _stringMap[iKey] = iValue;
    }

    const std::string StringStore::get(const std::string &iKey)
    {
        if (_stringMap.find(iKey) == _stringMap.end())
        {
            throw DataError("Invalid key, data not found");
        }
        else
        {
            return _stringMap[iKey];
        }
    }

    StringStore::~StringStore()
    {
        // call destructor;
    }

}