set (PROJECT zdaemon)

add_library (${PROJECT} SHARED daemon.cc daemon_exception.cc)

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${PROJECT} zlogger rt)

install(TARGETS ${PROJECT} DESTINATION /usr/local/lib)
install(DIRECTORY . DESTINATION /usr/local/include/zorg FILES_MATCHING PATTERN "*.h")
