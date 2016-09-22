set (PROJECT msg)

add_library (${PROJECT} SHARED logger.cc)

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${PROJECT})
