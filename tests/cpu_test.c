#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"

#define PRINT_ADDR 0x5 // The address the print function is expected by ROMs

static bool debug = false;

/* Simulates an OS print call that these early test ROMs expect. */
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

/* Prints useful debug information. */
void print_debug(CPU *cpu) {
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

/* Runs a test ROM. */
void run_test(const char *filename, int test_num) {
    printf("TEST %d:\n", test_num);

    CPU cpu;
    cpu_init(&cpu);

    if (!cpu_load_rom(&cpu, filename, 0x100)) {
        fprintf(stderr, "Unable to open %s\n", filename);
    }

    // These test ROMs expect to be loaded into 0x100
    cpu.pc = 0x100;

    /* Insert opcodes to handle simulated OS print call.
     * OP_OUT not really necessary, included to keep CPU in sync with
     * reference emulator for debugging purposes. */
    cpu.memory[PRINT_ADDR] = OP_OUT;
    cpu.memory[PRINT_ADDR + 2] = OP_RET;
    cpu.memory[0] = OP_EXIT;

    while(!cpu.exit) {
        if (cpu.instr_complete) {
            if (debug) {
                print_debug(&cpu);
            }

            // If the print function is called by ROM, explicitly call it here
            if (cpu.pc == PRINT_ADDR) {
                PRINT(&cpu);
            }
        }

        cpu_tick(&cpu);
    }

    printf("\n\n\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./intel-8080-tests <debug>\n");
    } else if (atoi(argv[1]) == 1) {
        debug = true;
    }

    run_test("../roms/tests/TST8080.COM", 1);
    run_test("../roms/tests/8080PRE.COM", 2);
    run_test("../roms/tests/CPUTEST.COM", 3);
    run_test("../roms/tests/8080EXM.COM", 4);
}
