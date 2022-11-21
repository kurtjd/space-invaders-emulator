cmake_minimum_required(VERSION 3.1)

project(Intel-8080-Emulator
    VERSION 1.0
    DESCRIPTION "Space Invaders and Stuff!"
    LANGUAGES C)

add_executable("intel-8080-tests"
    tests/cpu_test.c
    cpu/opcodes.c
    cpu/cpu.c)

target_include_directories("intel-8080-tests" PUBLIC cpu)
target_compile_options("intel-8080-tests" PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-result -std=c99)

add_executable("space-invaders"
    space-invaders/invaders.c
    cpu/opcodes.c
    cpu/cpu.c)

target_include_directories("space-invaders" PUBLIC cpu)
target_compile_options("space-invaders" PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-result -std=c99)