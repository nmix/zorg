set (PROJECT zdemo)

add_executable (${PROJECT} zdemo.cc)

target_link_libraries (${PROJECT} LINK_PUBLIC zlog)
