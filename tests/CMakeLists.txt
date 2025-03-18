set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.14)
project(SimpleGrammer C)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(LIBS "../libs/unity/unity.c")
file(GLOB SOURCES "src/*.c" "../inc/casm_dict.h" "../libs/klib/*.c")
add_executable(main ${SOURCES} ${LIBS})
target_include_directories(main PRIVATE
    ./inc
    ../libs/unity
    ../inc
    ../libs/klib
    ../libs/tiny_regex_c
    "C:/users/andre/scoop/apps/gcc/13.2.0/include/c++/13.2.0"
    "C:/users/andre/scoop/apps/gcc/13.2.0/include/c++/13.2.0/x86_64-w64-mingw32"
    "C:/users/andre/scoop/apps/gcc/13.2.0/x86_64-w64-mingw32/include"
    "C:/users/andre/scoop/apps/gcc/13.2.0/include/c++/13.2.0/backward"
    "C:/users/andre/scoop/apps/gcc/13.2.0/lib/gcc/x86_64-w64-mingw32/13.2.0/include"
)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/main ../resources/grammar.txt
    DEPENDS main
    COMMENT "running the program"
)

