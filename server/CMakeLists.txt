add_executable(server server.cpp backward.cpp Ice.cpp AuthenticationImpl.cpp SessionImpl.cpp SocketHandler.cpp ${out_file} ${copy_deps})

add_subdirectory(auth)

set(LIBS common ${LIBS})
target_link_libraries(server ${LIBS})
target_link_libraries(server common)
target_link_libraries(server ice)
target_link_libraries(server zmqpp)
target_link_libraries(server protobuf)
target_link_libraries(server auth)
