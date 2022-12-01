cmake_minimum_required(VERSION 3.16.3)
project(lab32)

set(CMAKE_CXX_STANDARD 98)

add_executable(lab32 proxy.c parser.h header.h network.h network.c cache.h cache.c parser.c pthreadSync.c pthreadSync.h connection.h connection.c)
target_link_libraries(lab32 pthread)
