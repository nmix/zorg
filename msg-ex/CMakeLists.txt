set (PROJECT dmx)

add_executable (${PROJECT} main.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC zdaemon ${Boost_LIBRARIES})

add_subdirectory (d1)
add_subdirectory (d2)
