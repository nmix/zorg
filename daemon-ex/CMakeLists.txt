set (PROJECT zdex)

add_executable (${PROJECT} main.cc exdaemon.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC zdaemon ${Boost_LIBRARIES})
