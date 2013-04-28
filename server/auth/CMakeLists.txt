add_library(auth Auth.cpp)
target_link_libraries(auth zmqpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")
