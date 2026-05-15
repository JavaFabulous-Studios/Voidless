cmake_minimum_required(VERSION 3.20)
project(VoidlessServer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for ENet
include(FetchContent)

FetchContent_Declare(
    enet
    GIT_REPOSITORY https://github.com/lsalzman/enet.git
    GIT_TAG master
)
FetchContent_MakeAvailable(enet)

# Server Executable
add_executable(VoidlessServer src/main.cpp)

target_include_directories(VoidlessServer PRIVATE 
    include
    ${enet_SOURCE_DIR}/include
)

target_link_libraries(VoidlessServer PRIVATE enet)

# Output directory
set_target_properties(VoidlessServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
