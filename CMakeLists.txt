# define name project
project(pathfinding)

# minimum version required
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

find_package(SFML REQUIRED network audio graphics window system)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

file( GLOB my_srcs *.cpp *.h *.hpp )
add_executable(${PROJECT_NAME} ${my_srcs})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
