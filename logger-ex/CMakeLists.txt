set (PROJECT log)

add_executable (${PROJECT} main.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC zlogger)
