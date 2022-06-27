#include <stdio.h>
#include "cpu.h"

int main(void) {
    CPU cpu;
    cpu_reset(&cpu);
    cpu_load_rom(&cpu, "../roms/test-rom/test.bin");

    printf("Hi I'm an Intel 8080 Emulator!\n\n");

    while (!cpu.exit) {
        cpu_tick(&cpu);
    }

    printf("\nEmulator exited.\n");
    return 0;
}
