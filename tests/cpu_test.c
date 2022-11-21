#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define PRINT_ADDR 0x5

void debug(CPU *cpu) {
    /*printf("State (%d):\n", cycles);
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
    printf("\n");*/

    printf("PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, CYC: %d (%s)\n",
          cpu->pc,
          (cpu->reg[A] << 8) | cpu_get_sw(cpu),
          (cpu->reg[B] << 8) | cpu->reg[C],
          (cpu->reg[D] << 8) | cpu->reg[E],
          (cpu->reg[H] << 8) | cpu->reg[L],
          cpu->sp,
          cpu->total_cycles,
          opcode_str[cpu->memory[cpu->pc]]);
}

void PRINT(const CPU *cpu) {
    if (cpu->reg[C] == 9) {
        char *str = (char *)&cpu->memory[cpu_get_reg_pair(cpu, D, E)];
        while (*str != '$') {
            printf("%c", *str++);
        }
    } else if (cpu->reg[C] == 2) {
        printf("%c", cpu->reg[E]);
    }
}

int main(void) {
    CPU cpu;
    cpu_init(&cpu);

    cpu_load_rom(&cpu, "../roms/tests/TST8080.COM", 0x100);
    cpu.pc = 0x100;
    cpu.memory[PRINT_ADDR] = 0xd3;
    cpu.memory[PRINT_ADDR + 2] = 0xc9;
    cpu.memory[0] = OP_EXIT;

    while(!cpu.exit) {
        if (cpu.instr_complete) {
            debug(&cpu);

            if (cpu.pc == PRINT_ADDR) {
                PRINT(&cpu);
            }
        }

        cpu_tick(&cpu);
    }

    printf("\n");
}
