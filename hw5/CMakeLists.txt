cmake_minimum_required(VERSION 3.30)
project(GtkTreeView C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(GtkTreeView main.c)

target_include_directories(GtkTreeView PRIVATE ${GLIB_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})
target_link_libraries(GtkTreeView PRIVATE ${GLIB_LIBRARIES} ${GTK3_LIBRARIES})

target_compile_options(GtkTreeView PRIVATE ${GTK3_CFLAGS_OTHER})