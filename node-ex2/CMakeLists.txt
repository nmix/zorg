set (PROJECT node)

add_executable (${PROJECT} main.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC znode ${Boost_LIBRARIES})
