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

# Fetch stb (for stb_image)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)
# Fetch GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8 # or another stable version
)
FetchContent_MakeAvailable(glm)
# Add stb_image as a library
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(2dengine main.cpp ${SOURCES})
target_link_libraries(2dengine PRIVATE glfw glad stb_image glm)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
