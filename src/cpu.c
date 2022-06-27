#include <stdio.h>
#include "cpu.h"

// Maps each opcode to its size in bytes
int opcode_size[NUM_OPCODES] = {
    1, // NOP
    3, // LXI
    1  // STAX
    // And so on...
};

/* Perhaps put all 'private' functions (ya know, functions that the user of the
 * cpu struct don't need to know, like factored out opcode functions) here.
 * IE:
 * 
 * static void _NOP(void) {
 *     // do nothing!
 * }
 * 
 */

void cpu_reset(CPU *cpu) {
    // Helper stuff
    cpu->exit = false;

    // Clear Registers
    cpu->pc = 0;
    cpu->sp = 0;
    for (int i = 0; i < NUM_REGS; i++) {
        cpu->reg[i] = 0;
    }

    // Clear RAM
    for (int i = 0; i < MAX_RAM; i++) {
        cpu->ram[i] = 0;
    }
}

bool cpu_load_rom(CPU *cpu, const char *filename) {
    FILE *rom = fopen(filename, "rb");

    if (rom) {
        fread(cpu->ram, MAX_RAM, 1, rom);
        fclose(rom);
        return true;
    }

    fprintf(stderr, "Unable to open ROM file %s\n", filename);
    return false;
}

void cpu_tick(CPU *cpu) {
    // Since opcodes can be upto 3 bytes long, fetch the next 3 bytes
    uint8_t bytes[3] = {
        cpu->ram[cpu->pc],
        cpu->ram[cpu->pc + 1],
        cpu->ram[cpu->pc + 2]
    };

    // Manual states PC is incremented before execution (CHIP-8 was like this too)
    cpu->pc += opcode_size[bytes[0]];

    // Begin long switch here?
    switch (bytes[0]) {
        case NOP:
            printf("NOP!\n");
            break;
        // And so on...

        // FOR TESTING ONLY! Just borrowing this opcode to tell main to exit
        case RST:
            cpu->exit = true;
            break;

        default:
            break;
    }
}
