add_library(spdlog INTERFACE IMPORTED GLOBAL)
add_library(eigen3 INTERFACE IMPORTED GLOBAL)

target_include_directories(spdlog INTERFACE spdlog/include)
target_include_directories(eigen3 INTERFACE Eigen)
