cmake_minimum_required(VERSION 3.4)
project(ssh_manager)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.c
    sshpass.c
    Node.c
    Node.h)


add_executable(ssh_manager ${SOURCE_FILES})
target_link_libraries(ssh_manager ncurses menu)