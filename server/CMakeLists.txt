include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/server/src/*.cpp")
add_executable(server main.cpp ${SOURCES} ${CMAKE_SOURCE_DIR}/src/physics/physics.cpp ${CMAKE_SOURCE_DIR}/src/network/network.cpp ${CMAKE_SOURCE_DIR}/src/logger.cpp)
target_include_directories(server PRIVATE ${chipmunk_SOURCE_DIR}/include)
target_include_directories(server PRIVATE ${enet_SOURCE_DIR}/include)
target_link_libraries(server PRIVATE glm chipmunk enet)
