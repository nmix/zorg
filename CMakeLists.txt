cmake_minimum_required(VERSION 3.5) 

project (ZORG)

set (Boost_USE_STATIC_LIBS        ON)

find_package(Boost 1.61 COMPONENTS log log_setup thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions("-std=c++11 -Wextra -Werror")

add_subdirectory (zlog)
add_subdirectory (zlog-demo)
