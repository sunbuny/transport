cmake_minimum_required(VERSION 3.25)
include (CheckIncludeFiles)
project (transport)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(tools)

check_include_files(ifaddrs.h HAVE_IFADDRS_H)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.hh.in src/config.h)


########################################
# The Google Protobuf library for message generation + serialization
find_package(Protobuf REQUIRED)
if (NOT PROTOBUF_FOUND)
  BUILD_ERROR ("Missing: Google Protobuf")
endif()
if (NOT PROTOBUF_PROTOC_EXECUTABLE)
  BUILD_ERROR ("Missing: Google Protobuf Compiler")
endif()
if (NOT PROTOBUF_PROTOC_LIBRARY)
  BUILD_ERROR ("Missing: Google Protobuf Compiler Library")
endif()

find_package(cppzmq CONFIG REQUIRED)

# Create the transport library
add_library(transport
        src/transport.cpp
        src/socket.cpp
        src/netUtils.cpp
        src/packet.cpp
        src/topicsInfo.cpp
        src/uuid.cpp
)
target_include_directories(transport PUBLIC ./include)
target_link_libraries(transport PUBLIC
        protobuf::libprotobuf
        cppzmq
)

target_compile_definitions(transport
        PUBLIC
        -D_CRT_SECURE_NO_WARNINGS          # remove secure warnings (e.g sprinft_s)
        -DNOMINMAX                         # remove min/max macros
        -D_WINSOCK_DEPRECATED_NO_WARNINGS  # remove winsock deprecated warnings
)
target_link_libraries(transport PUBLIC wsock32 )

# Unit tests


add_executable(UNIT_packet_TEST test/packet_TEST.cpp)
add_executable(UNIT_topicsInfo_TEST test/topicsInfo_TEST.cpp)
add_executable(UNIT_discZmq_TEST test/discZmq_TEST.cpp)

target_link_libraries(UNIT_packet_TEST disczmq gtest gtest_main)
target_link_libraries(UNIT_topicsInfo_TEST disczmq gtest gtest_main)
target_link_libraries(UNIT_discZmq_TEST disczmq gtest gtest_main)
