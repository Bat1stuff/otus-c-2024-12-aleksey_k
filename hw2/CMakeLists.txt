cmake_minimum_required(VERSION 3.30)
project(hw2 C)

set(CMAKE_C_STANDARD 11)

find_package(CURL REQUIRED)
find_package(json-c REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(hw2 weather_api.c)

target_link_libraries(hw2 ${CURL_LIBRARIES})
target_link_libraries(hw2 json-c::json-c)