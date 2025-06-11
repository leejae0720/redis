cmake_minimum_required(VERSION 3.10)
project(redis_pubsub)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(/usr/local/lib)

add_executable(publisher src/publisher.cpp)
add_executable(subscriber src/subscriber.cpp)

find_library(HIREDIS_LIB hiredis)
target_link_libraries(publisher ${HIREDIS_LIB})
target_link_libraries(subscriber ${HIREDIS_LIB})
