
cmake_minimum_required(VERSION 2.8)

set(app_name "GreedySnake")

project(${app_name} CXX)

set(SRC_DIR "${CMAKE_SOURCE_DIR}")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(LIB_DIR "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g")

# find_package(SDL2 REQUIRED)

include_directories(
    # ${SDL2_INCLUDE_DIRS}
    include
)

file(GLOB_RECURSE cpps "src/*.cpp")

add_executable(${app_name} ${cpps})
target_link_libraries(${app_name}
    # ${SDL2_LIBRARIES}
    SDL2_image SDL2_ttf SDL2
)

set(res "${BIN_DIR}/res")
add_custom_command(OUTPUT "${res}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SRC_DIR}/res" "${res}"
)
add_custom_target(run
    COMMAND ./${app_name}
    WORKING_DIRECTORY "${BIN_DIR}"
    DEPENDS "${res}"
)

