cmake_minimum_required(VERSION 3.10)
project(imageprocessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# --verbose flag might be helpful in case something is broken during linking
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-ggdb3 -Wall -Werror -Wno-write-strings -D_GLIBCXX_DEBUG")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")


find_program(CCACHE ccache)
if(CCACHE)
	message("using ccache")
	set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
	message("ccache not found! Not usable")
endif()

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

add_subdirectory(3rdParty)

file(GLOB SOURCES "src/*.cpp")
include_directories(src)
add_executable(imageprocessing ${SOURCES})
target_link_libraries(imageprocessing PUBLIC -pthread spdlog eigen3 ${GLEW_LIBRARIES} -lGLU -lGL glfw)
