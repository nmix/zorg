set (PROJECT zdln)

add_library (${PROJECT} SHARED node_daemon.cc master_node_daemon.cc slave_node_daemon.cc)

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${PROJECT} zdaemon zlogger znode ${Boost_LIBRARIES})
