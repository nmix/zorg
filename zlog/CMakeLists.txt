
set (PROJECT zlog)

# file (GLOB CPPS *.cc)
# set (SOURCE ${CPPS})

# set (BOOST_ROOT /usr/local/boost_1_61_0)
# add_definitions("-std=c++11 -Wextra -Werror -DBOOST_LOG_DYN_LINK")
# add_definitions("-std=c++11 -Wextra -Werror")

# include_directories(${BOOST_ROOT})
# link_directories(/usr/local/lib)

add_library (${PROJECT} zlogger.cc)
target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add_executable(${EXE} main.cc) 
target_link_libraries (${PROJECT} pthread ${Boost_LIBRARIES})
