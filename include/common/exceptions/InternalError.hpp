#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace redislite
{
    class InternalError : public std::runtime_error
    {
    public:
        explicit InternalError(const std::string &message)
            : std::runtime_error(message) {}

        explicit InternalError(const std::stringstream &message)
            : std::runtime_error(message.str()) {}

        explicit InternalError(const char *message)
            : std::runtime_error(message) {}
    };
}
