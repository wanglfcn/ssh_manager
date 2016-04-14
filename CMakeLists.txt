cmake_minimum_required(VERSION 3.4)
project(gt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.c
    sshpass.c
    Node.c
    Node.h)


add_executable(gt ${SOURCE_FILES})
target_link_libraries(gt ncurses menu)