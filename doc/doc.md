# RedisLite - In-Memory Key-Value Store

## Overview
RedisLite is a lightweight, in-memory key-value store that mimics the functionality of Redis. The system is designed to work in a single-threaded, non-blocking mode, allowing multiple clients to interact with the server simultaneously without the need for multithreading.

The system uses non-blocking socket programming with `poll()` to handle multiple connections efficiently in an event-driven manner. The core functionality includes simple key-value operations such as `put` and `get`.

---

## Architecture

RedisLite follows an **event-driven, single-threaded architecture** with the following key components:

1. **Client**:
   - Responsible for establishing a connection to the RedisLite server, sending commands, and receiving responses.
   - Uses non-blocking socket operations and `poll()` for efficient communication.

2. **Server**:
   - Listens for incoming client connections, processes commands, and sends responses.
   - Manages multiple clients using `poll()` to handle read/write readiness and ensures non-blocking operations.

3. **StringStore**:
   - The in-memory key-value store, implemented using a `std::unordered_map`.
   - Stores data in memory and provides methods to store (`put()`) and retrieve (`get()`) key-value pairs.

4. **Logger**:
   - Logs significant events and errors to a log file with timestamps for debugging and performance tracking.
   - Implements the singleton pattern to ensure a single instance of the logger is used across the application.

5. **GenericSingleton**:
   - A generic singleton template used to create a single instance of classes like `Logger` and `StringStore`.

---

## Modules

### 1. Client Module

#### Header File: `Client.hpp`

- **Purpose**: Handles the client-side logic for connecting to the RedisLite server, sending commands, and receiving responses.
  
- **Key Functions**:
  - `run()`: The main event loop that uses `poll()` to handle read and write events. It checks if the socket is ready to send or receive data and invokes `sendData()` or `readData()` accordingly.
  - `setNonBlocking()`: Sets the client socket to non-blocking mode.
  - `connectToServer()`: Connects the client to the server using non-blocking sockets and `poll()` for waiting for the connection.
  - `sendData()`: Sends user input as a command to the server.
  - `readData()`: Reads data from the server and displays it to the user.

#### File: `Client.cpp`

- **Implementation Details**:
  - The client socket is created and configured as non-blocking using `fcntl()`.
  - Uses `poll()` to check for both read and write readiness on the socket.
  - Sends commands input by the user, reads responses from the server, and gracefully handles errors and disconnections.

### 2. Server Module

#### Header File: `Server.hpp`

- **Purpose**: Handles the server-side logic for accepting incoming client connections, processing commands, and sending responses back to clients.
  
- **Key Functions**:
  - `run()`: The main event loop that waits for incoming client connections and processes client commands.
  - `setNonBlocking()`: Sets the server socket to non-blocking mode.
  - `acceptNewClient()`: Accepts new incoming client connections and adds them to the poll list.
  - `removeClient()`: Removes a client from the poll list when they disconnect or encounter an error.
  - `handleClient()`: Reads data from a client, processes commands, and sends responses.

#### File: `Server.cpp`

- **Implementation Details**:
  - The server socket is created and configured as non-blocking.
  - Uses `poll()` to manage multiple clients efficiently, with separate handling for accepting new clients and reading data from existing clients.
  - Each client command is processed using a command handler (`CmdHandler`), and responses are sent back to clients.

### 3. StringStore Module

#### Purpose: 
  - An in-memory key-value store implemented using `std::unordered_map`. It stores key-value pairs in memory, providing fast access for retrieval and insertion.

#### File: `StringStore.hpp` and `StringStore.cpp`

- **Key Functions**:
  - `put()`: Stores a key-value pair in the store.
  - `get()`: Retrieves the value associated with a given key.

#### Implementation Details:
  - The data store is a simple unordered map that provides constant-time complexity (O(1)) for `put()` and `get()` operations.
  - The `StringStore` class is designed to be accessed globally as a singleton instance (through `GenericSingleton`).

### 4. Logger Module

#### Header File: `Logger.hpp`

- **Purpose**: Provides logging functionality to record important events and errors in the application, written to a log file.

#### File: `Logger.cpp`

- **Key Functions**:
  - `log()`: Logs a message with a timestamp to a log file.
  
- **Implementation Details**:
  - The `Logger` class uses the `std::ofstream` to write logs to a file (`redislite.log`). Each log message is timestamped using `std::chrono`.
  - Implements the Singleton pattern to ensure a single instance is used throughout the application.

### 5. Generic Singleton

#### Header File: `redislite/GenericSingleton.hpp`

- **Purpose**: Implements a generic singleton pattern that ensures only one instance of a given class is created.

- **Implementation Details**:
  - The `GenericSingleton` class template ensures that a class (such as `Logger` or `StringStore`) has only one instance throughout the application's lifetime.
  - It uses a static instance of the template class to provide access via `getInstance()`.

---

## Key Design Patterns Used

1. **Singleton Pattern**: 
   - Used in the `Logger` and `StringStore` classes to ensure only one instance exists for the application. This is important for managing shared resources like logs and in-memory data.

2. **Non-Blocking I/O & Event-Driven Architecture**: 
   - The `Client` and `Server` modules use non-blocking sockets and `poll()` to manage multiple connections efficiently in a single thread. The event-driven model allows simultaneous read and write operations without blocking the main execution thread.

---

## Networking and Socket Programming

- **Non-blocking Sockets**: Both the server and client use non-blocking sockets to avoid blocking operations while waiting for network events.
- **`poll()`**: The `poll()` system call is used in both the server and client modules to monitor multiple file descriptors (sockets) for readiness to perform I/O operations (reading or writing).
- **Event Handling**: The server handles multiple clients by checking which sockets are ready for reading or writing, ensuring that no socket is blocked unnecessarily.

---

## Error Handling

- Custom error handling is provided by throwing `InternalError` exceptions with meaningful error messages. These exceptions are used in critical areas such as socket creation, connection handling, and data reading/writing.

---

## Conclusion

RedisLite is a highly modular, efficient, and lightweight implementation of a key-value store, designed to work in a single-threaded environment using non-blocking sockets and an event-driven model. The architecture is scalable and easy to maintain, making it suitable for embedded systems or other resource-constrained environments. The key design patterns used, such as Singleton and event-driven I/O, contribute to the system's flexibility and performance.
****