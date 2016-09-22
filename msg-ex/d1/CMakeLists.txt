set (PROJECT d1)

add_executable (${PROJECT} main.cc exdaemon1.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC zdaemon zmq ${Boost_LIBRARIES})
