

# Binaries
add_executable(publisher publisher.cpp)
add_executable(subscriber subscriber.cpp)
add_executable(requester requester.cpp)
add_executable(replier replier.cpp)
add_executable(test_pub test_pub.cpp)
add_executable(test_sub test_sub.cpp)

target_link_libraries(publisher ${Boost_LIBRARIES} disczmq protobuf boost_program_options)
target_link_libraries(subscriber ${Boost_LIBRARIES} disczmq protobuf boost_program_options)
target_link_libraries(requester ${Boost_LIBRARIES} disczmq protobuf boost_program_options)
target_link_libraries(replier ${Boost_LIBRARIES} disczmq protobuf boost_program_options)

target_link_libraries(test_pub  transport)
target_link_libraries(test_sub  transport)


