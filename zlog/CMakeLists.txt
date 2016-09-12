cmake_minimum_required(VERSION 3.5) 

set (PROJECT zlog)


file (GLOB CPPS *.cc)
set (SOURCE ${CPPS})

set (BOOST_ROOT /usr/local/boost_1_61_0)
add_definitions("-std=c++11 -Wextra -Werror -DBOOST_LOG_DYN_LINK")

include_directories(${BOOST_ROOT})
link_directories(/usr/local/lib)

add_executable(${PROJECT} ${SOURCE}) 
target_link_libraries (${PROJECT} boost_log boost_log_setup pthread boost_thread boost_system)
