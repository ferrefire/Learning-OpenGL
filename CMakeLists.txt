cmake_minimum_required(VERSION 3.28)
project(learingOpenGL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SOURCE_FILES src/*.cpp src/*.c)

include_directories(includes)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(GLFW_BUILD_WAYLAND False)
add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)