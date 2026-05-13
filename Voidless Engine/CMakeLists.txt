cmake_minimum_required(VERSION 3.20)
project(VoidlessEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define directories
set(ENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include FetchContent for dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    GIT_SHALLOW TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glad)

# Engine Library
add_library(VoidlessEngine STATIC
    ${ENGINE_SRC_DIR}/main.cpp
    ${ENGINE_SRC_DIR}/Window.cpp
)

target_include_directories(VoidlessEngine PUBLIC 
    ${ENGINE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_link_libraries(VoidlessEngine PUBLIC 
    glfw
    glm::glm
    glad
)

# Test Executable
add_executable(VoidlessEngineTest ${ENGINE_SRC_DIR}/main.cpp)
target_link_libraries(VoidlessEngineTest PRIVATE VoidlessEngine)
