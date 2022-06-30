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
 * For example NOP appears to be 0x00, LXI_B 0x01, STAX_B 0x02, etc.
 * And enum values start at 0 by default.
 */
typedef enum {
    NOP,
    LXI_B,
    STAX_B,
    INX_B,
    INR_B,
    DCR_B,
    MVI_B,
    RLC,
    UNDEFINED_1,
    DAD_B,
    LDAX_B,
    DCX_B,
    INR_C,
    DCR_C,
    MVI_C,
    RRC,
    UNDEFINED_2,
    LXI_D,
    STAX_D,
    INX_D,
    INR_D,
    DCR_D,
    MVI_D,
    RAL,
    UNDEFINED_3,
    DAD_D,
    LDAX_D,
    DCX_D,
    INR_E,
    DCR_E,
    MVI_E,
    RAR,
    UNDEFINED_4,
    LXI_H,
    SHLD,
    INX_H,
    INR_H,
    DCR_H,
    MVI_H,
    DAA,
    UNDEFINED_5,
    DAD_H,
    LHLD,
    DCX_H,
    INR_L,
    DCR_L,
    MVI_L,
    CMA,
    UNDEFINED_6,
    LXI_SP,
    STA,
    INX_SP,
    INR_M,
    DCR_M,
    MVI_M,
    STC,
    UNDEFINED_7,
    DAD_SP,
    LDA,
    DCX_SP,
    INR_A,
    DCR_A,
    MVI_A,
    CMC,
    MOV_B_B,
    MOV_B_C,
    MOV_B_D,
    MOV_B_E,
    MOV_B_H,
    MOV_B_L,
    MOV_B_M,
    MOV_B_A,
    MOV_C_B,
    MOV_C_C,
    MOV_C_D,
    MOV_C_E,
    MOV_C_H,
    MOV_C_L,
    MOV_C_M,
    MOV_C_A,
    MOV_D_B,
    MOV_D_C,
    MOV_D_D,
    MOV_D_E,
    MOV_D_H,
    MOV_D_L,
    MOV_D_M,
    MOV_D_A,
    MOV_E_B,
    MOV_E_C,
    MOV_E_D,
    MOV_E_E,
    MOV_E_H,
    MOV_E_L,
    MOV_E_M,
    MOV_E_A,
    MOV_H_B,
    MOV_H_C,
    MOV_H_D,
    MOV_H_E,
    MOV_H_H,
    MOV_H_L,
    MOV_H_M,
    MOV_H_A,
    MOV_L_B,
    MOV_L_C,
    MOV_L_D,
    MOV_L_E,
    MOV_L_H,
    MOV_L_L,
    MOV_L_M,
    MOV_L_A,
    MOV_M_B,
    MOV_M_C,
    MOV_M_D,
    MOV_M_E,
    MOV_M_H,
    MOV_M_L,
    HLT,
    MOV_M_A,
    MOV_A_B,
    MOV_A_C,
    MOV_A_D,
    MOV_A_E,
    MOV_A_H,
    MOV_A_L,
    MOV_A_M,
    MOV_A_A,
    ADD_B,
    ADD_C,
    ADD_D,
    ADD_E,
    ADD_H,
    ADD_L,
    ADD_M,
    ADD_A,
    ADC_B,
    ADC_C,
    ADC_D,
    ADC_E,
    ADC_H,
    ADC_L,
    ADC_M,
    ADC_A,
    SUB_B,
    SUB_C,
    SUB_D,
    SUB_E,
    SUB_H,
    SUB_L,
    SUB_M,
    SUB_A,
    SBB_B,
    SBB_C,
    SBB_D,
    SBB_E,
    SBB_H,
    SBB_L,
    SBB_M,
    SBB_A,
    ANA_B,
    ANA_C,
    ANA_D,
    ANA_E,
    ANA_H,
    ANA_L,
    ANA_M,
    ANA_A,
    XRA_B,
    XRA_C,
    XRA_D,
    XRA_E,
    XRA_H,
    XRA_L,
    XRA_M,
    XRA_A,
    ORA_B,
    ORA_C,
    ORA_D,
    ORA_E,
    ORA_H,
    ORA_L,
    ORA_M,
    ORA_A,
    CMP_B,
    CMP_C,
    CMP_D,
    CMP_E,
    CMP_H,
    CMP_L,
    CMP_M,
    CMP_A,
    RNZ,
    POP_B,
    JNZ,
    JMP,
    CNZ,
    PUSH_B,
    ADI,
    RST_0,
    RZ,
    RET,
    JZ,
    UNDEFINED_8,
    CZ,
    CALL,
    ACI,
    RST_1,
    RNC,
    POP_D,
    JNC,
    OUT,
    CNC,
    PUSH_D,
    SUI,
    RST_2,
    RC,
    UNDEFINED_9,
    JC,
    IN,
    CC,
    UNDEFINED_10,
    SBI,
    RST_3,
    RPO,
    POP_H,
    JPO,
    XTHL,
    CPO,
    PUSH_H,
    ANI,
    RST_4,
    RPE,
    PCHL,
    JPE,
    XCHG,
    CPE,
    UNDEFINED_11,
    XRI,
    RST_5,
    RP,
    POP_PSW,
    JP,
    DI,
    CP,
    PUSH_PSW,
    ORI,
    RST_6,
    RM,
    SPHL,
    JM,
    EI,
    CM,
    EXIT, // Orginally undefined so using this to signal to main to exit
    CPI,
    RST_7
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


/* Maps each status flag "mnemonic" to it's corresponding bit.
 * These mnemonics are lifted from the manual itself.
 * There doesn't appear to be a specific bit mapping of flags so I think
 * it's just left up to us.
 */
typedef enum {
    CY = 1 << 0,  // Carry Bit
    AC = 1 << 1,  // Aux Carry Bit
    S =  1 << 2,  // Sign Bit
    Z =  1 << 3,  // Zero Bit
    P =  1 << 4   // Parity Bit
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


// Initialize the CPU (essentially init opcode sizes then reset)
void cpu_init(CPU *cpu);

// Performs a CPU reset as if a RESET signal was sent.
void cpu_reset(CPU *cpu);

// Loads a ROM file into memory
bool cpu_load_rom(CPU *cpu, const char *filename);

// Executes the next instruction
void cpu_tick(CPU *cpu);

#endif
