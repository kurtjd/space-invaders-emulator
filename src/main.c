#include <stdio.h>
#include "cpu.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./intel-8080-emu <path-to-rom>\n");
        return 1;
    }

    CPU cpu;
    cpu_init(&cpu);

    if (!cpu_load_rom(&cpu, argv[1])) {
        fprintf(stderr, "Unable to open ROM file %s\n", argv[1]);
        return 1;
    }

    printf("Beginning execution of ROM...\n\n");

    while (!cpu.exit) {
        cpu_tick(&cpu);
    }

    printf("\nEmulator exited.\n");
    return 0;
}
