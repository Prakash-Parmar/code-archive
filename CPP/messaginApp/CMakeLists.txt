cmake_minimum_required(VERSION 3.10)
project(MessaginApp)

# Use C++17 for better standard library features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find our headers
include_directories(include)

# 1. Output Organization (MUST be set before add_executable/add_library)
# Binaries go to build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Static libraries go to build/lib/
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 2. Create a library for our reusable Socket logic
add_library(networking_lib src/socket.cpp)

# 3. Build the Server executable
add_executable(server src/server.cpp)
target_link_libraries(server networking_lib)

# 4. Build the Client executable
add_executable(client src/client.cpp)
target_link_libraries(client networking_lib)
