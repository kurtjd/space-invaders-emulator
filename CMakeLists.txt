cmake_minimum_required(VERSION 3.1)

project(Intel-8080-Emulator
    VERSION 1.0
    DESCRIPTION "Space Invaders and Stuff!"
    LANGUAGES C)

add_executable("intel-8080-emu"
    src/main.c
    src/cpu.c)

target_include_directories("intel-8080-emu" PUBLIC include)
target_compile_options("intel-8080-emu" PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-result)

add_executable("intel-8080-tests"
    tests/cpu_test.c
    src/cpu.c)

target_include_directories("intel-8080-tests" PUBLIC include)
target_compile_options("intel-8080-tests" PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-result)