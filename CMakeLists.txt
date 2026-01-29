cmake_minimum_required(VERSION 3.10.0)
project(2dengine VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/scenes)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find FreeType from system or fetch if not found
find_package(Freetype QUIET)
if(NOT Freetype_FOUND)
    message(STATUS "FreeType not found, fetching with FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(freetype)
    set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include)
    add_library(Freetype::Freetype ALIAS freetype)
endif()

# Find GLFW from system or fetch if not found
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found, fetching with FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)
endif()

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
include(FetchContent)

FetchContent_Declare(
    chipmunk
    GIT_REPOSITORY https://codeberg.org/slembcke/Chipmunk2D.git
    GIT_TAG master
)
FetchContent_MakeAvailable(chipmunk)
FetchContent_Declare(
    enet
    GIT_REPOSITORY https://github.com/lsalzman/enet.git
    GIT_TAG v1.3.18
)

# Disable ENet examples/tests
set(ENET_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ENET_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(enet)
# Add stb_image as a library
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB SOURCES_PHYS "${CMAKE_SOURCE_DIR}/src/physics/*.cpp")
file(GLOB SOURCES_NET "${CMAKE_SOURCE_DIR}/src/network/*.cpp")
file(GLOB SCENES "${CMAKE_SOURCE_DIR}/src/scenes/*.cpp")
add_executable(2dengine main.cpp ${SOURCES} ${SCENES} ${SOURCES_PHYS} ${SOURCES_NET})
target_include_directories(2dengine PRIVATE ${chipmunk_SOURCE_DIR}/include)
target_include_directories(2dengine PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(2dengine PRIVATE glfw glad stb_image glm chipmunk enet Freetype::Freetype)
target_include_directories(2dengine PRIVATE ${enet_SOURCE_DIR}/include)
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
add_subdirectory(${CMAKE_SOURCE_DIR}/server)
add_custom_command(TARGET 2dengine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shader files..."
)
add_custom_command(TARGET 2dengine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts
    ${CMAKE_BINARY_DIR}/fonts
    COMMENT "Copying font files..."
)
