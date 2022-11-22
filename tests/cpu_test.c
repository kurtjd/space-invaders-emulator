#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"

#define PRINT_ADDR 0x5 // The address the print function is expected by ROMs

CPU cpu;
static bool debug = false;
static bool quit = false;

void EXIT_TEST(uint8_t data) {
    (void)data;
    quit = true;
}

/* Simulates an OS print call that these early test ROMs expect. */
void PRINT(uint8_t data) {
    (void)data;
    if (cpu.reg[C] == 9) {
        char *str = (char *)&cpu.memory[cpu_get_reg_pair(&cpu, D, E)];
        while (*str != '$') {
            printf("%c", *str++);
        }
    } else if (cpu.reg[C] == 2) {
        printf("%c", cpu.reg[E]);
    }
}

/* Runs a test ROM. */
void run_test(const char *filename, int test_num) {
    printf("TEST %d:\n", test_num);

    cpu_init(&cpu);

    if (!cpu_load_rom(&cpu, filename, 0x100)) {
        fprintf(stderr, "Unable to open %s\n", filename);
    }

    // These test ROMs expect to be loaded into 0x100
    cpu.pc = 0x100;

    // Inject opcodes to simulate OS calls
    cpu.output[0] = EXIT_TEST;
    cpu.memory[0] = OP_OUT;
    cpu.memory[1] = 0x00;

    cpu.output[1] = PRINT;
    cpu.memory[PRINT_ADDR] = OP_OUT;
    cpu.memory[PRINT_ADDR + 1] = 0x01;
    cpu.memory[PRINT_ADDR + 2] = OP_RET;

    unsigned long num_instr = 0;
    while(!cpu.exit) {
        if (cpu.instr_complete) {
            if (debug && num_instr == 1000000000) {
                cpu_print_debug(&cpu);
                getchar();
            }
            if (quit) {
                quit = false;
                break;
            }

            num_instr++;
        }

        cpu_tick(&cpu);
    }

    printf("\n\nInstructions: %lu\n", num_instr);
    printf("Total Cycles: %lu\n\n\n", cpu.total_cycles);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./intel-8080-tests <debug>\n");
    } else if (atoi(argv[1]) == 1) {
        debug = true;
    }

    //run_test("../roms/tests/TST8080.COM", 1);
    //run_test("../roms/tests/8080PRE.COM", 2);
    //run_test("../roms/tests/CPUTEST.COM", 3);
    run_test("../roms/tests/8080EXM.COM", 4);
}
