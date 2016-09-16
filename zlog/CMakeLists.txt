set (PROJECT zlog)

add_library (${PROJECT} SHARED zlogger.cc)

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${PROJECT} pthread ${Boost_LIBRARIES})
