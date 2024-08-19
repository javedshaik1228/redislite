#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace redislite
{
    class DataError : public std::runtime_error
    {
    public:
        explicit DataError(const std::string &message)
            : std::runtime_error(message) {}
    };
}
