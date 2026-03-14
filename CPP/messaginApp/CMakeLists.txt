cmake_minimum_required(VERSION 3.10)
project(MessaginApp)

# Use C++17 for better standard library features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find our headers
include_directories(include)

# 1. Output Organization
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 2. Create a library for our reusable Networking logic
add_library(networking_lib 
    src/socket.cpp
)

# 3. Build the Server executable
add_executable(server 
    src/chat_server.cpp 
    src/server_main.cpp
)
target_link_libraries(server networking_lib)

# 4. Build the Client executable
add_executable(client 
    src/chat_client.cpp 
    src/client_main.cpp
)
target_link_libraries(client networking_lib)
