#include <utils/Logger.hpp>

#include <iostream>
#include <iomanip>
#include <chrono>

std::ofstream Logger::_logFile;

Logger::Logger()
{
    _logFile.open(LOGFILE_PATH, std::ios::app);
    if (!_logFile.is_open())
    {
        std::cerr << "Failed to open log file!" << std::endl;
        throw std::runtime_error("Error while opening log file");
    }
}

void Logger::log(const std::stringstream &message)
{
    // std::cout << message.str() << std::endl;

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm *tm_now = std::localtime(&now_c);

    _logFile << "[" << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S") << "] " << message.str() << std::endl;
}

Logger::~Logger()
{
    if (_logFile.is_open())
    {
        _logFile.close();
    }
}