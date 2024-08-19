#pragma once

#include <GenericSingleton.hpp>
#include <string>
#include <unordered_map>

namespace redislite
{

    class StringStore : public GenericSingleton<StringStore>
    {
        friend class GenericSingleton<StringStore>;

    public:
        void put(const std::string &iKey, const std::string &iValue);
        const std::string get(const std::string &iKey);

    private:
        StringStore();
        StringStore &operator=(const StringStore &) = delete;
        ~StringStore();

        static std::unordered_map<std::string, std::string> _stringMap;
    };
}