
add_executable(chat-server "server.cpp")
add_executable(chat-client "client.cpp")

target_link_libraries(chat-server PRIVATE crib)
target_link_libraries(chat-client PRIVATE crib)
