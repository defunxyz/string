cmake_minimum_required(VERSION 3.8...3.26)
project(string VERSION 0.0.0 PROJECT_DESCRIPTION "")

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/string/string.hpp)

add_executable(string_prototype EXCLUDE_FROM_ALL main.cpp)
