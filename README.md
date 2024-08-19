# redislite

An in memory database built with C++

## Prerequisites
- CMake (version 3.10 or higher)
- C++17 or later

## Installation
To build from source, you require cmake

```sh
mkdir build && cd $_
cmake ..
make
```
## Run
Start server:
```sh
./bin/redislite_server
```
Client:
```sh
./bin/redislite_client
```

## Todo
- Volatile data that expire when given a TTL
- Persistent data storage
- Add more datatypes