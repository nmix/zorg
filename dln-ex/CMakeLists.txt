set (PROJECT dln)

add_executable (${PROJECT} main.cc main_daemon.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC zdln ${Boost_LIBRARIES})
