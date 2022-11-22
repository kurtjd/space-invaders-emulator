#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>


#define NUM_OPCODES 0x100
#define MAX_OPERANDS 2
#define NUM_REGS 8
#define MAX_MEM 0x10000
#define NUM_IO 0xFF


/* This array maps opcodes to a string.
 * Useful for tests and eventual disassembler.
 */
extern const char *opcode_str[NUM_OPCODES];

/* This array maps opcodes to their number of cycles.
*/
extern const int opcode_cycles[NUM_OPCODES];

/* Maps each opcode to its size in bytes. Technically opcodes are 1 byte but
 * they may expect 1-2 bytes as "operands".
 */
extern int opcode_size[NUM_OPCODES];


/* This enum associates each mnemonic with it's actual binary opcode.
 * Could just be helpful for making switch more readable.
 * For example NOP appears to be 0x00, LXI_B 0x01, STAX_B 0x02, etc.
 * And enum values start at 0 by default.
 */
typedef enum {
    OP_NOP,
    OP_LXI_B,
    OP_STAX_B,
    OP_INX_B,
    OP_INR_B,
    OP_DCR_B,
    OP_MVI_B,
    OP_RLC,
    OP_UNDEFINED_1,
    OP_DAD_B,
    OP_LDAX_B,
    OP_DCX_B,
    OP_INR_C,
    OP_DCR_C,
    OP_MVI_C,
    OP_RRC,
    OP_UNDEFINED_2,
    OP_LXI_D,
    OP_STAX_D,
    OP_INX_D,
    OP_INR_D,
    OP_DCR_D,
    OP_MVI_D,
    OP_RAL,
    OP_UNDEFINED_3,
    OP_DAD_D,
    OP_LDAX_D,
    OP_DCX_D,
    OP_INR_E,
    OP_DCR_E,
    OP_MVI_E,
    OP_RAR,
    OP_UNDEFINED_4,
    OP_LXI_H,
    OP_SHLD,
    OP_INX_H,
    OP_INR_H,
    OP_DCR_H,
    OP_MVI_H,
    OP_DAA,
    OP_UNDEFINED_5,
    OP_DAD_H,
    OP_LHLD,
    OP_DCX_H,
    OP_INR_L,
    OP_DCR_L,
    OP_MVI_L,
    OP_CMA,
    OP_UNDEFINED_6,
    OP_LXI_SP,
    OP_STA,
    OP_INX_SP,
    OP_INR_M,
    OP_DCR_M,
    OP_MVI_M,
    OP_STC,
    OP_UNDEFINED_7,
    OP_DAD_SP,
    OP_LDA,
    OP_DCX_SP,
    OP_INR_A,
    OP_DCR_A,
    OP_MVI_A,
    OP_CMC,
    OP_MOV_B_B,
    OP_MOV_B_C,
    OP_MOV_B_D,
    OP_MOV_B_E,
    OP_MOV_B_H,
    OP_MOV_B_L,
    OP_MOV_B_M,
    OP_MOV_B_A,
    OP_MOV_C_B,
    OP_MOV_C_C,
    OP_MOV_C_D,
    OP_MOV_C_E,
    OP_MOV_C_H,
    OP_MOV_C_L,
    OP_MOV_C_M,
    OP_MOV_C_A,
    OP_MOV_D_B,
    OP_MOV_D_C,
    OP_MOV_D_D,
    OP_MOV_D_E,
    OP_MOV_D_H,
    OP_MOV_D_L,
    OP_MOV_D_M,
    OP_MOV_D_A,
    OP_MOV_E_B,
    OP_MOV_E_C,
    OP_MOV_E_D,
    OP_MOV_E_E,
    OP_MOV_E_H,
    OP_MOV_E_L,
    OP_MOV_E_M,
    OP_MOV_E_A,
    OP_MOV_H_B,
    OP_MOV_H_C,
    OP_MOV_H_D,
    OP_MOV_H_E,
    OP_MOV_H_H,
    OP_MOV_H_L,
    OP_MOV_H_M,
    OP_MOV_H_A,
    OP_MOV_L_B,
    OP_MOV_L_C,
    OP_MOV_L_D,
    OP_MOV_L_E,
    OP_MOV_L_H,
    OP_MOV_L_L,
    OP_MOV_L_M,
    OP_MOV_L_A,
    OP_MOV_M_B,
    OP_MOV_M_C,
    OP_MOV_M_D,
    OP_MOV_M_E,
    OP_MOV_M_H,
    OP_MOV_M_L,
    OP_HLT,
    OP_MOV_M_A,
    OP_MOV_A_B,
    OP_MOV_A_C,
    OP_MOV_A_D,
    OP_MOV_A_E,
    OP_MOV_A_H,
    OP_MOV_A_L,
    OP_MOV_A_M,
    OP_MOV_A_A,
    OP_ADD_B,
    OP_ADD_C,
    OP_ADD_D,
    OP_ADD_E,
    OP_ADD_H,
    OP_ADD_L,
    OP_ADD_M,
    OP_ADD_A,
    OP_ADC_B,
    OP_ADC_C,
    OP_ADC_D,
    OP_ADC_E,
    OP_ADC_H,
    OP_ADC_L,
    OP_ADC_M,
    OP_ADC_A,
    OP_SUB_B,
    OP_SUB_C,
    OP_SUB_D,
    OP_SUB_E,
    OP_SUB_H,
    OP_SUB_L,
    OP_SUB_M,
    OP_SUB_A,
    OP_SBB_B,
    OP_SBB_C,
    OP_SBB_D,
    OP_SBB_E,
    OP_SBB_H,
    OP_SBB_L,
    OP_SBB_M,
    OP_SBB_A,
    OP_ANA_B,
    OP_ANA_C,
    OP_ANA_D,
    OP_ANA_E,
    OP_ANA_H,
    OP_ANA_L,
    OP_ANA_M,
    OP_ANA_A,
    OP_XRA_B,
    OP_XRA_C,
    OP_XRA_D,
    OP_XRA_E,
    OP_XRA_H,
    OP_XRA_L,
    OP_XRA_M,
    OP_XRA_A,
    OP_ORA_B,
    OP_ORA_C,
    OP_ORA_D,
    OP_ORA_E,
    OP_ORA_H,
    OP_ORA_L,
    OP_ORA_M,
    OP_ORA_A,
    OP_CMP_B,
    OP_CMP_C,
    OP_CMP_D,
    OP_CMP_E,
    OP_CMP_H,
    OP_CMP_L,
    OP_CMP_M,
    OP_CMP_A,
    OP_RNZ,
    OP_POP_B,
    OP_JNZ,
    OP_JMP,
    OP_CNZ,
    OP_PUSH_B,
    OP_ADI,
    OP_RST_0,
    OP_RZ,
    OP_RET,
    OP_JZ,
    OP_UNDEFINED_8,
    OP_CZ,
    OP_CALL,
    OP_ACI,
    OP_RST_1,
    OP_RNC,
    OP_POP_D,
    OP_JNC,
    OP_OUT,
    OP_CNC,
    OP_PUSH_D,
    OP_SUI,
    OP_RST_2,
    OP_RC,
    OP_UNDEFINED_9,
    OP_JC,
    OP_IN,
    OP_CC,
    OP_UNDEFINED_10,
    OP_SBI,
    OP_RST_3,
    OP_RPO,
    OP_POP_H,
    OP_JPO,
    OP_XTHL,
    OP_CPO,
    OP_PUSH_H,
    OP_ANI,
    OP_RST_4,
    OP_RPE,
    OP_PCHL,
    OP_JPE,
    OP_XCHG,
    OP_CPE,
    OP_UNDEFINED_11,
    OP_XRI,
    OP_RST_5,
    OP_RP,
    OP_POP_PSW,
    OP_JP,
    OP_DI,
    OP_CP,
    OP_PUSH_PSW,
    OP_ORI,
    OP_RST_6,
    OP_RM,
    OP_SPHL,
    OP_JM,
    OP_EI,
    OP_CM,
    OP_EXIT, // Orginally undefined so using this to signal to main to exit
    OP_CPI,
    OP_RST_7
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
/*typedef enum {
    CY = 1 << 0,  // Carry Bit
    AC = 1 << 1,  // Aux Carry Bit
    S =  1 << 2,  // Sign Bit
    Z =  1 << 3,  // Zero Bit
    P =  1 << 4   // Parity Bit
} FLAG_BITS;*/
typedef enum {
    S   = 1 << 5,  // Sign Bit
    Z   = 1 << 4,  // Zero Bit
    AC  = 1 << 3,  // Aux Carry Bit
    P   = 1 << 2,  // Parity Bit
    CY  = 1 << 1,  // Carry Bit
    I   = 1 << 0   // Interrupt Bit
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
    uint8_t (*input[NUM_IO])(void);
    void (*output[NUM_IO])(uint8_t);

    // Helper stuff
    bool exit; // Signals main to exit loop
    bool halt; // Signals CPU to halt until interrupt
    uint32_t total_cycles;
    uint32_t instr_cycles;
    uint32_t cycle_cum;
    bool instr_complete;
} CPU;


// Initialize the CPU (essentially init opcode sizes then reset)
void cpu_init(CPU *cpu);

// Performs a CPU reset as if a RESET signal was sent.
void cpu_reset(CPU *cpu);

// Loads a ROM file into memory
bool cpu_load_rom(CPU *cpu, const char *filename, uint16_t start_addr);

// Retrieves the specified flag bit
bool cpu_get_flag_bit(const CPU *cpu, FLAG_BITS flag);

// Sets the specified flag bit
void cpu_set_flag_bit(CPU *cpu, FLAG_BITS flag, bool high);

// Returns a uint16_t with reg1 as the MSB and reg2 as the LSB
uint16_t cpu_get_reg_pair(const CPU *cpu, REGISTERS reg1, REGISTERS reg2);

// Given a uint16_t, places the MSB in reg1 and the LSB in reg2
void cpu_set_reg_pair(CPU *cpu, REGISTERS reg1, REGISTERS reg2, uint16_t val);

// Returns the status word of the CPU
uint8_t cpu_get_sw(const CPU *cpu);

// Generates an interrupt request
void cpu_interrupt(CPU *cpu, uint8_t int_num);

// Prints useful debug information
void cpu_print_debug(const CPU *cpu);

// Executes the next instruction
void cpu_tick(CPU *cpu);

#endif
