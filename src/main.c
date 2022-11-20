#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

void debug(CPU *cpu, int cycles) {
    printf("State (%d):\n", cycles);
    printf("AF: %04x   BC: %04x   DE: %04x   HL: %04x   SP: %04x\n",
          (cpu->reg[A] << 8) | cpu->reg[FLAGS],
          (cpu->reg[B] << 8) | cpu->reg[C],
          (cpu->reg[D] << 8) | cpu->reg[E],
          (cpu->reg[H] << 8) | cpu->reg[L],
          cpu->sp);
    printf("Flags: %c %c %c %c %c\n\n",
           cpu_get_flag_bit(cpu, P) ? 'p' : '.',
           cpu_get_flag_bit(cpu, Z) ? 'Z' : '.',
           cpu_get_flag_bit(cpu, S) ? 's' : '.',
           cpu_get_flag_bit(cpu, AC) ? 'a' : '.',
           cpu_get_flag_bit(cpu, CY) ? 'c' : '.');
    printf("%04x %04x %s",
           cpu->pc, cpu->memory[cpu->pc], opcode_str[cpu->memory[cpu->pc]]);
    getchar();
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./intel-8080-emu <path-to-rom> <num-instructions>\n");
        return 1;
    }

    CPU cpu;
    cpu_init(&cpu);

    if (!cpu_load_rom(&cpu, argv[1], 0)) {
        fprintf(stderr, "Unable to open ROM file %s\n", argv[1]);
        return 1;
    }

    int cycles = 0;
    int num_instructions = atoi(argv[2]);

    printf("Beginning execution of ROM...\n\n");

    while (!cpu.exit) {
        if (cycles >= num_instructions) {
            debug(&cpu, cycles);
        }

        cpu_tick(&cpu);
        cycles++;
    }

    printf("\nEmulator exited.\n");
    return 0;
}
