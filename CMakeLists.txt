project(demo_jrb
        LANGUAGES C)

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)
message(STATUS "Include: ${GTK3_INCLUDE_DIRS}\n LIB_DIR: ${GTK3_LIBRARY_DIRS} \n CFLAGS_OTHERS: ${GTK3_CFLAGS_OTHER} \n LIBS: ${GTK3_LIBRARIES}")

include_directories(inc)
file(GLOB SOURCES "src-lib/*c" "src-main/*.c")
add_library(inc STATIC ${SOURCES})
add_executable(dic dic.c)

add_executable(data data.c)
target_link_libraries(data inc)
target_link_libraries(dic ${GTK3_LIBRARIES} inc)



