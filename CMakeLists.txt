cmake_minimum_required(VERSION 3.14)

project(lmath)
add_library(${PROJECT_NAME} INTERFACE)
add_subdirectory(Src/lmath)
target_include_directories(${PROJECT_NAME} INTERFACE ./Src)