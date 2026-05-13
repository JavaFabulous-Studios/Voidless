cmake_minimum_required(VERSION 3.20)
project(VoidlessGame VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define directories
set(GAME_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find Engine
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Voidless Engine)
add_subdirectory("${ENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/engine")

# Game Executable
add_executable(Voidless ${GAME_SRC_DIR}/main.cpp)

# Link Engine
target_link_libraries(Voidless PRIVATE VoidlessEngine)

# Copy shaders or assets if needed
# add_custom_command(...)
