cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compilers and linker
# set(CMAKE_C_LINKER lld)
# set(CMAKE_C_FLAGS "-O3")
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "-Og -g -gdwarf")

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_C_FLAGS "--target=x86_64-w64-mingw32 -O3 -ffast-math")

# Project configuration
project(casm_test C)

set(CMAKE_BUILD_TYPE Debug)

set(LIBS "../libs/unity/unity.c" "../libs/tiny_regex_c/re.c")
file(GLOB SOURCES "../src/*.c" "src/*.c" "../libs/klib/*.c")
list(FILTER SOURCES EXCLUDE REGEX ".*casm_main\\.c$")

add_executable(${PROJECT_NAME} ${SOURCES} ${LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE
    ./inc
    ../libs/unity
    ../inc
    ../libs/klib
    ../libs/tiny_regex_c
    # "C:/Users/andre/scoop/apps/llvm/19.1.7/lib/clang/19/include"
    "C:/Users/andre/scoop/apps/mingw/14.2.0-rt_v12-rev1/lib/gcc/x86_64-w64-mingw32/14.2.0/include"
    "C:/Users/andre/scoop/apps/mingw/14.2.0-rt_v12-rev1/x86_64-w64-mingw32/include"
)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME} ../resources/grammar.txt
    DEPENDS ${PROJECT_NAME}
    COMMENT "running the program"
)

if(TESTING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TESTING)
endif()