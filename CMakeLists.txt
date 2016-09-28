cmake_minimum_required(VERSION 3.5)

project (ZORG)

add_definitions("-std=c++11 -Wextra -Werror")

find_package(Boost 1.61 COMPONENTS log log_setup thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} include)

add_subdirectory (logger)
add_subdirectory (logger-ex)
add_subdirectory (daemon)
add_subdirectory (daemon-ex)
add_subdirectory (node)
add_subdirectory (node-ex)
add_subdirectory (node-ex2)
add_subdirectory (dln)
add_subdirectory (dln-ex)
