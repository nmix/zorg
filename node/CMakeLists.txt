set (PROJECT znode)

add_library (${PROJECT} SHARED node.cc node_exception.cc master_node.cc slave_node.cc)

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${PROJECT} zmq)
