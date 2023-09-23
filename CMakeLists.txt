cmake_minimum_required(VERSION 3.21)
add_library(LMath INTERFACE)
add_subdirectory(Src/LMath)
target_include_directories(LMath INTERFACE Src)