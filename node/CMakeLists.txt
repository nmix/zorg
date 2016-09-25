set (PROJECT znode)

add_library (${PROJECT} SHARED master_node.cc node.cc node_exception.cc)

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${PROJECT} zmq)
