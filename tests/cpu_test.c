#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define PRINT_ADDR 0x5

void debug(CPU *cpu) {
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

    cpu_load_rom(&cpu, "../roms/tests/CPUTEST.COM", 0x100);
    cpu.pc = 0x100;
    cpu.memory[PRINT_ADDR] = 0xd3;
    cpu.memory[PRINT_ADDR + 2] = 0xc9;
    cpu.memory[0] = OP_EXIT;

    while(!cpu.exit) {
        if (cpu.instr_complete) {
            //debug(&cpu);

            if (cpu.pc == PRINT_ADDR) {
                PRINT(&cpu);
            }
        }

        cpu_tick(&cpu);
    }

    printf("\n");
}
