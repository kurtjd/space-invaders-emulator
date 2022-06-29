#include <stdio.h>
#include "cpu.h"

/* Maps each opcode to its size in bytes. Technically opcodes are 1 byte but
 * they may expect 1-2 bytes as "operands".
 */
int opcode_size[NUM_OPCODES];


/*** PRIVATE FUNCTIONS ***/
/* Perhaps put all 'private' functions (ya know, functions that the user of the
 * cpu struct don't need to know, like factored out opcode functions) here.
 * IE:
 * 
 * static void _NOP(void) {
 *     // do nothing!
 * }
 * 
 */

/* Initializes the opcode_size array to make incrementing PC simple. */
static void _init_opcode_size(void) {
    /* Since the majority of opcodes are only 1 byte, default initialize
     * all to size 1. Unfortunately pure C has no sexy way to default initialize
     * to any value other than 0 that I'm aware of hence the for loop.
     */
    for (int i = 0; i < NUM_OPCODES; i++) {
        opcode_size[i] = 1;
    }

    // Now explicitly initialize any opcodes with size >1
    opcode_size[LXI] = 3;
    // And so on...
}

/* Performs a CPU reset as if a RESET signal was sent. */
static void _cpu_reset(CPU *cpu) {
    /* According to the manual, only the PC is set to 0 upon issue of a RESET.
     * Every other register "remains indeterminate, until initialized explicitly
     * by the program" however I don't see any downside to resetting them and
     * would be useful for running unit tests.
     */
    cpu->pc = 0;
    cpu->sp = 0;
    for (int i = 0; i < NUM_REGS; i++) {
        cpu->reg[i] = 0;
    }

    // Clear memory (probably not necessary really)
    for (int i = 0; i < MAX_MEM; i++) {
        cpu->memory[i] = 0;
    }
}

/* Retrieves the specified flag bit. */
static bool _get_flag_bit(const CPU *cpu, FLAG_BITS flag) {
    return cpu->reg[FLAGS] & flag;
}

/* Sets the specified flag bit. */
static void _set_flag_bit(CPU *cpu, FLAG_BITS flag, bool high) {
    if (high) {
        cpu->reg[FLAGS] |= flag;
    } else {
        cpu->reg[FLAGS] &= ~flag;
    }
}


/*** PUBLIC FUNCTIONS ***/
void cpu_init(CPU *cpu) {
    _init_opcode_size();
    _cpu_reset(cpu);
    cpu->exit = false;
}

bool cpu_load_rom(CPU *cpu, const char *filename) {
    FILE *rom = fopen(filename, "rb");

    if (rom) {
        fread(cpu->memory, MAX_MEM, 1, rom);
        fclose(rom);
        return true;
    }

    return false;
}

void cpu_tick(CPU *cpu) {
    uint8_t opcode = cpu->memory[cpu->pc];

    /* In case we run into an opcode that expects 1-2 bytes as "operands"
     * (Such as a 2-byte address)
     */
    uint8_t operands[2] = {
        cpu->memory[cpu->pc + 1],
        cpu->memory[cpu->pc + 2]
    };

    /* Manual states PC is incremented before execution. Order is important
     * because some opcodes may change the PC.
     */
    cpu->pc += opcode_size[opcode];

    /* Begin long switch here?
     * Might also be useful to organize these into the five "groups":
     * 
     * -Data Transfer
     * -Arithmetic
     * -Logical
     * -Branch
     * -Stack/IO
     */
    switch (opcode) {
        case NOP:
            printf("NOP!\n");
            break;

        case LXI:
            // Do stuff
            break;

        case STAX:
            // Do stuff
            break;
        // And so on...

        /* FOR TESTING ONLY! Just borrowing this opcode to tell main to exit.
         * In reality this is used in conjunction with interrupts.
         */
        case HLT:
            cpu->exit = true;
            break;

        default:
            break;
    }
}
