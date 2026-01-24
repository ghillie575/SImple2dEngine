cmake_minimum_required(VERSION 3.10.0)
project(2dengine VERSION 0.1.0 LANGUAGES C CXX)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find GLFW from system
find_package(glfw3 REQUIRED)

# Fetch GLAD
include(FetchContent)
FetchContent_Declare(glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glad)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(2dengine main.cpp ${SOURCES})
target_link_libraries(2dengine PRIVATE glfw glad)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
