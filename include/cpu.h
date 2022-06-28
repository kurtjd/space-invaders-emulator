#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>


#define NUM_OPCODES 0xFF
#define NUM_REGS 8
#define MAX_MEM 0xFFFF
#define NUM_IO 0xFF


// This might be useful to outside users (specifically the test suite)
extern int opcode_size[NUM_OPCODES];


/* This enum associates each mnemonic with it's actual binary opcode.
 * Could just be helpful for making switch more readable.
 * For example NOP appears to be 0x00, LXI 0x01, STAX 0x02, etc.
 * And enum values start at 0 by default.
 */
typedef enum {
    NOP,
    LXI,
    STAX,
    // And so on...

    HLT = 0x76
} OPCODES;


// Maps register "mnemonics" to an actual register number
typedef enum {
    B,
    C,
    D,
    E,
    H,
    L,
    A,      // Accumulator
    FLAGS   // 5-Bit Status Flags
} REGISTERS;


/* Maps each status flag "mnemonic" to it's corresponding bit number.
 * These mnemonics are lifted from the manual itself.
 * There doesn't appear to be a specific bit mapping of flags so I think
 * it's just left up to us.
 */
typedef enum {
    CY, // Carry Bit
    AC, // Aux Carry Bit
    S,  // Sign Bit
    Z,  // Zero Bit
    P   // Parity Bit
} FLAG_BITS;


// Represents the CPU of the emulator including memory
typedef struct CPU {
    uint16_t pc; // Program counter register
    uint16_t sp; // Stack pointer register
    uint8_t reg[NUM_REGS]; // 8-bit registers

    uint8_t memory[MAX_MEM]; // Memory, including data and code

    /* Memory-mapped IO (subject to change, might be better to just use the
     * existing memory array).
     */
    uint8_t input[NUM_IO];
    uint8_t output[NUM_IO];

    // Helper stuff
    bool exit; // Signals main to exit loop
} CPU;


// Initialize the CPU
void cpu_init(CPU *cpu);

// Loads a ROM file into memory
bool cpu_load_rom(CPU *cpu, const char *filename);

// Executes the next instruction
void cpu_tick(CPU *cpu);

#endif
