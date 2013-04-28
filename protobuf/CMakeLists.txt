# add any ice files here
set(PROTOBUF_SOURCE_FILES AuthMsg AuthReply MessageEvent)

# don't edit below
set(PROTO_CPP_FILES)

foreach(proto_file ${PROTOBUF_SOURCE_FILES})

  set(out_dir "${CMAKE_CURRENT_BINARY_DIR}")
  set(out_file "${out_dir}/${proto_file}.pb.cc")

  add_custom_command(
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.proto"
    OUTPUT "${out_file}"
    COMMAND "protoc"
      --proto_path="${CMAKE_CURRENT_SOURCE_DIR}"
      "${CMAKE_CURRENT_SOURCE_DIR}/*.proto"
      --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
  )

  set(PROTO_CPP_FILES ${PROTO_CPP_FILES} ${out_file})
endforeach()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(protobuf ${PROTO_CPP_FILES})
