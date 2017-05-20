cmake_minimum_required(VERSION 3.7)
project(proxy)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        my_exception.cpp
        my_exception.h
        my_kqueue.cpp
        my_kqueue.h
        smart_fd.cpp
        smart_fd.h
        socket.cpp
        socket.h timer.cpp timer.h server.cpp server.h)

add_executable(proxy ${SOURCE_FILES})