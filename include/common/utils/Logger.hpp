#pragma once

#include <GenericSingleton.hpp>

#include <fstream>
#include <sstream>

namespace
{
    const std::string LOGFILE_PATH = "./redislite.log";
}

class Logger : public GenericSingleton<Logger>
{
    friend class GenericSingleton<Logger>;

public:
    void log(const std::stringstream &message);

private:
    Logger();
    static std::ofstream _logFile;
    Logger &operator=(const Logger &) = delete;
    ~Logger();
};