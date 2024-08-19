#pragma once

#include <iostream>
#include <utils/Logger.hpp>

#define LOG_MSG(message)                \
    do                                  \
    {                                   \
        std::stringstream ss;           \
        ss << "MSG: " << message;       \
        Logger::getInstance()->log(ss); \
    } while (0)

#define LOG_ERROR(message)              \
    do                                  \
    {                                   \
        std::stringstream ss;           \
        ss << "ERR: " << message;       \
        Logger::getInstance()->log(ss); \
    } while (0)

// Log a message to console and log file
#define LOG_CSL_MSG(message)               \
    do                                     \
    {                                      \
        std::stringstream ss;              \
        ss << "MSG: " << message;          \
        Logger::getInstance()->log(ss);    \
        std::cout << message << std::endl; \
    } while (0)

// Log an error to console and log file
#define LOG_CSL_ERR(message)               \
    do                                     \
    {                                      \
        std::stringstream ss;              \
        ss << "ERR: " << message;          \
        Logger::getInstance()->log(ss);    \
        std::cout << message << std::endl; \
    } while (0)

#define THROW_RUNTIME(message)              \
    do                                      \
    {                                       \
        std::stringstream ss;               \
        ss << "EXC: " << message;           \
        Logger::getInstance()->log(ss);     \
        throw std::runtime_error(ss.str()); \
    } while (0)

// END