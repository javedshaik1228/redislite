cmake_minimum_required(VERSION 3.10)

project(RedisLite VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Common source files
set(COMMON_SOURCES
    src/common/utils/Logger.cpp
)

# Add server executable
add_executable(redislite_server
    src/server/main.cpp
    src/server/Server.cpp
    src/data/StringStore.cpp
    src/common/CmdHandler.cpp
    ${COMMON_SOURCES}
)

# Add client executable
add_executable(redislite_client
    src/client/main.cpp
    src/client/Client.cpp
    ${COMMON_SOURCES}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/exceptions
)

# Include directories
target_include_directories(redislite_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/data
)
target_include_directories(redislite_client PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
)

set_target_properties(redislite_server redislite_client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)