cmake_minimum_required(VERSION 3.20)

project(arm-mimic.elf VERSION 0.0.1 LANGUAGES CXX)

find_package(libhal-lpc40xx REQUIRED CONFIG)


add_link_options(-lc -lrdimon -u _printf_float)
add_executable(${PROJECT_NAME} main.cpp newlib.cpp initializer.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PRIVATE libhal::lpc4078)
arm_cortex_post_build(${PROJECT_NAME})
