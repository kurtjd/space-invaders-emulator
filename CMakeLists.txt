cmake_minimum_required(VERSION 3.1)

project(Space-Invaders-Emulator
    VERSION 1.0
    DESCRIPTION "Space Invaders and Intel 8080 Emulator"
    LANGUAGES C)

add_executable("cpu-tests"
    tests/cpu_test.c
    cpu/opcodes.c
    cpu/cpu.c)

target_include_directories("cpu-tests" PUBLIC cpu)
target_compile_options("cpu-tests" PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-unused-parameter)

add_executable("invaders"
    space-invaders/invaders.c
    space-invaders/hardware.c
    cpu/opcodes.c
    cpu/cpu.c)

target_include_directories("invaders" PUBLIC cpu hardware)
target_compile_options("invaders" PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-unused-parameter)
target_link_libraries("invaders" -lSDL2 SDL2_mixer)