

# Binaries

add_executable(requester requester.cpp)
add_executable(replier replier.cpp)
add_executable(pub pub.cpp)
add_executable(sub sub.cpp)

target_link_libraries(pub  transport)
target_link_libraries(sub  transport)
target_link_libraries(requester  transport)
target_link_libraries(replier  transport)


