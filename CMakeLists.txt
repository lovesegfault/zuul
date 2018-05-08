cmake_minimum_required(VERSION 3.5)
project(zuul C)

set(CMAKE_C_STANDARD 99)

if(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    set(SANITIZE "-fsanitize=address,undefined,leak,cfi -flto -fvisibility=hidden")
    set(DEBUG_OPT "-O1")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(SANITIZE "-fsanitize=address,undefined,leak")
    set(DEBUG_OPT "-Og")
endif()

set(WARNINGS "-Wall -Wextra -Wformat-security -Wmissing-include-dirs -Wuninitialized -Wno-unused-parameter -Werror=delete-non-virtual-dtor -Werror=return-type -Werror=implicit")
set(TUNE "-mtune=native -march=native")

set(CMAKE_C_FLAGS_DEBUG "-std=c99 -g ${DEBUG_OPT} ${SANITIZE} ${WARNINGS}")
set(CMAKE_C_FLAGS_RELEASE "-std=c99 -O3 -flto ${TUNE} ${WARNINGS}")

find_library(pthread_lib pthread)

# zuul
add_library(zuul STATIC src/zuul.c src/fifo.c src/fifo.h src/printc.c src/printc.h)
set_target_properties(zuul PROPERTIES COMPILE_FLAGS "-DTESTS_DISABLED")
target_link_libraries(zuul pthread_lib)

# tests
add_executable(tests src/tests.c)
target_link_libraries(tests zuul)