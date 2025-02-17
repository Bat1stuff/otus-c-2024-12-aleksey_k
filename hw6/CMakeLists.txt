cmake_minimum_required(VERSION 3.30)
project(project_with_log_lib C)

add_library(logger STATIC ${CMAKE_SOURCE_DIR}/include/library.c)

set(MY_LOGGER_LIB logger)

set(CMAKE_C_STANDARD 11)

add_executable(project_with_log_lib main.c)

target_link_libraries(project_with_log_lib ${MY_LOGGER_LIB})

set(CMAKE_C_FLAGS -rdynamic)