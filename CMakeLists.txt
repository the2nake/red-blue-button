cmake_minimum_required(VERSION 3.20)

project(RedBlueButton LANGUAGES CXX)

# find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

set(EXEC_NAME "red-blue-button")

file(GLOB SRC_FILES "src/*.cpp")
add_executable(${EXEC_NAME} ${SRC_FILES})

# target_link_libraries(${EXEC_NAME} PRIVATE SDL3::SDL3)
set_target_properties(${EXEC_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED FALSE
)
