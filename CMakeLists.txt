cmake_minimum_required(VERSION 3.13)
project(imageprocessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -Wno-write-strings -D_GLIBCXX_DEBUG")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

set(EigenHeaders "3rdParties/Eigen/Eigen")
file(GLOB eigen_include_directories ${EigenHeaders})
include_directories(${eigen_include_directories})

file(GLOB SOURCES "src/*.cpp")
include_directories(include)
add_executable(imageprocessing ${SOURCES})
target_link_libraries(imageprocessing ${GLEW_LIBRARIES} -lGLU -lGL glfw)
