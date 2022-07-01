#include <stdio.h>
#include "cpu.h"

const char *opcode_str[NUM_OPCODES] = {
    "NOP",
    "LXI B",
    "STAX B",
    "INX B",
    "INR B",
    "DCR B",
    "MVI B",
    "RLC",
    "UNDEFINED 1",
    "DAD B",
    "LDAX B",
    "DCX B",
    "INR C",
    "DCR C",
    "MVI C",
    "RRC",
    "UNDEFINED 2",
    "LXI D",
    "STAX D",
    "INX D",
    "INR D",
    "DCR D",
    "MVI D",
    "RAL",
    "UNDEFINED 3",
    "DAD D",
    "LDAX D",
    "DCX D",
    "INR E",
    "DCR E",
    "MVI E",
    "RAR",
    "UNDEFINED 4",
    "LXI H",
    "SHLD",
    "INX H",
    "INR H",
    "DCR H",
    "MVI H",
    "DAA",
    "UNDEFINED 5",
    "DAD H",
    "LHLD",
    "DCX H",
    "INR L",
    "DCR L",
    "MVI L",
    "CMA",
    "UNDEFINED 6",
    "LXI SP",
    "STA",
    "INX SP",
    "INR M",
    "DCR M",
    "MVI M",
    "STC",
    "UNDEFINED 7",
    "DAD SP",
    "LDA",
    "DCX SP",
    "INR A",
    "DCR A",
    "MVI A",
    "CMC",
    "MOV B, B",
    "MOV B, C",
    "MOV B, D",
    "MOV B, E",
    "MOV B, H",
    "MOV B, L",
    "MOV B, M",
    "MOV B, A",
    "MOV C, B",
    "MOV C, C",
    "MOV C, D",
    "MOV C, E",
    "MOV C, H",
    "MOV C, L",
    "MOV C, M",
    "MOV C, A",
    "MOV D, B",
    "MOV D, C",
    "MOV D, D",
    "MOV D, E",
    "MOV D, H",
    "MOV D, L",
    "MOV D, M",
    "MOV D, A",
    "MOV E, B",
    "MOV E, C",
    "MOV E, D",
    "MOV E, E",
    "MOV E, H",
    "MOV E, L",
    "MOV E, M",
    "MOV E, A",
    "MOV H, B",
    "MOV H, C",
    "MOV H, D",
    "MOV H, E",
    "MOV H, H",
    "MOV H, L",
    "MOV H, M",
    "MOV H, A",
    "MOV L, B",
    "MOV L, C",
    "MOV L, D",
    "MOV L, E",
    "MOV L, H",
    "MOV L, L",
    "MOV L, M",
    "MOV L, A",
    "MOV M, B",
    "MOV M, C",
    "MOV M, D",
    "MOV M, E",
    "MOV M, H",
    "MOV M, L",
    "HLT",
    "MOV M, A",
    "MOV A, B",
    "MOV A, C",
    "MOV A, D",
    "MOV A, E",
    "MOV A, H",
    "MOV A, L",
    "MOV A, M",
    "MOV A, A",
    "ADD B",
    "ADD C",
    "ADD D",
    "ADD E",
    "ADD H",
    "ADD L",
    "ADD M",
    "ADD A",
    "ADC B",
    "ADC C",
    "ADC D",
    "ADC E",
    "ADC H",
    "ADC L",
    "ADC M",
    "ADC A",
    "SUB B",
    "SUB C",
    "SUB D",
    "SUB E",
    "SUB H",
    "SUB L",
    "SUB M",
    "SUB A",
    "SBB B",
    "SBB C",
    "SBB D",
    "SBB E",
    "SBB H",
    "SBB L",
    "SBB M",
    "SBB A",
    "ANA B",
    "ANA C",
    "ANA D",
    "ANA E",
    "ANA H",
    "ANA L",
    "ANA M",
    "ANA A",
    "XRA B",
    "XRA C",
    "XRA D",
    "XRA E",
    "XRA H",
    "XRA L",
    "XRA M",
    "XRA A",
    "ORA B",
    "ORA C",
    "ORA D",
    "ORA E",
    "ORA H",
    "ORA L",
    "ORA M",
    "ORA A",
    "CMP B",
    "CMP C",
    "CMP D",
    "CMP E",
    "CMP H",
    "CMP L",
    "CMP M",
    "CMP A",
    "RNZ",
    "POP B",
    "JNZ",
    "JMP",
    "CNZ",
    "PUSH B",
    "ADI",
    "RST 0",
    "RZ",
    "RET",
    "JZ",
    "UNDEFINED 8",
    "CZ",
    "CALL",
    "ACI",
    "RST 1",
    "RNC",
    "POP D",
    "JNC",
    "OUT",
    "CNC",
    "PUSH D",
    "SUI",
    "RST 2",
    "RC",
    "UNDEFINED 9",
    "JC",
    "IN",
    "CC",
    "UNDEFINED 10",
    "SBI",
    "RST 3",
    "RPO",
    "POP H",
    "JPO",
    "XTHL",
    "CPO",
    "PUSH H",
    "ANI",
    "RST 4",
    "RPE",
    "PCHL",
    "JPE",
    "XCHG",
    "CPE",
    "UNDEFINED 11",
    "XRI",
    "RST 5",
    "RP",
    "POP PSW",
    "JP",
    "DI",
    "CP",
    "PUSH PSW",
    "ORI",
    "RST 6",
    "RM",
    "SPHL",
    "JM",
    "EI",
    "CM",
    "EXIT",
    "CPI",
    "RST 7"
};

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

    // Explicitly initialize any opcodes with size 2
    opcode_size[MVI_B] = 2;
    opcode_size[MVI_C] = 2;
    opcode_size[MVI_D] = 2;
    opcode_size[MVI_E] = 2;
    opcode_size[MVI_H] = 2;
    opcode_size[MVI_L] = 2;
    opcode_size[MVI_M] = 2;
    opcode_size[MVI_A] = 2;
    opcode_size[ADI]   = 2;
    opcode_size[ACI]   = 2;
    opcode_size[OUT]   = 2;
    opcode_size[SUI]   = 2;
    opcode_size[IN]    = 2;
    opcode_size[SBI]   = 2;
    opcode_size[ANI]   = 2;
    opcode_size[XRI]   = 2;
    opcode_size[ORI]   = 2;
    opcode_size[CPI]   = 2;

    // Explicitly initialize any opcodes with size 3
    opcode_size[LXI_B]  = 3;
    opcode_size[LXI_D]  = 3;
    opcode_size[LXI_H]  = 3;
    opcode_size[SHLD]   = 3;
    opcode_size[LHLD]   = 3;
    opcode_size[LXI_SP] = 3;
    opcode_size[STA]    = 3;
    opcode_size[LDA]    = 3;
    opcode_size[JNZ]    = 3;
    opcode_size[JMP]    = 3;
    opcode_size[CNZ]    = 3;
    opcode_size[JZ]     = 3;
    opcode_size[CZ]     = 3;
    opcode_size[CALL]   = 3;
    opcode_size[JNC]    = 3;
    opcode_size[CNC]    = 3;
    opcode_size[JC]     = 3;
    opcode_size[CC]     = 3;
    opcode_size[JPO]    = 3;
    opcode_size[CPO]    = 3;
    opcode_size[JPE]    = 3;
    opcode_size[CPE]    = 3;
    opcode_size[JP]     = 3;
    opcode_size[CP]     = 3;
    opcode_size[JM]     = 3;
    opcode_size[CM]     = 3;
}


/* Unfortunately, unlike the other status flags, CY and AC are manipulated
 * slightly differently depending on what instruction calls them, hence why
 * there are a few variations here. Most add instructions should be able to call
 * the add variants (except DAD rp (add register pairs) since that works on
 * 16-bit operands) and most sub instructions should be able to call the sub
 * variants. Finally, most logical instructions should be able to just call the
 * clear variant (except the rotate instructions). However, there are several
 * exceptions to these rules so look closely at how each instruction manipulates
 * these flags.
 */

/* Adds two values (plus the CY flag if wanted) and if the result is greater
 * than 8-bits than we know a carry out occurred.
 */
static void _update_flag_cy_add(CPU *cpu, uint8_t val1, uint8_t val2, bool add_carry) {
    int carry = add_carry ? cpu_get_flag_bit(cpu, CY) : 0;
    cpu_set_flag_bit(cpu, CY, val1 + val2 + carry > 0xFF);
}

/* Adds the first four bits of two values (plus the CY flag if wanted) and
 * if the result is greater than 0xF we know a carry out of the first 4 bits
 * occurred.
 */
static void _update_flag_ac_add(CPU *cpu, uint8_t val1, uint8_t val2, bool add_carry) {
    int carry = add_carry ? cpu_get_flag_bit(cpu, CY) : 0;
    cpu_set_flag_bit(cpu, AC, (val1 & 0xF) + (val2 & 0xF) + carry > 0xF);
}

/* Subtracts the second value from the first value by adding the two's
 * complement of the second value to the first (plus the CY flag if wanted) 
 * and if the result is greater than 0xFF we know a borrow out occurred.
 */
static void _update_flag_cy_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool sub_borrow) {
    int borrow = sub_borrow ? cpu_get_flag_bit(cpu, CY) : 0;
    cpu_set_flag_bit(cpu, CY, val1 + (~(val2 + borrow) + 1) > 0xFF);
}

/* Subtracts the first four bits of the second value from the first four bits
 * of the first value by adding the two's complement of the second value to the
 * first (plus the CY flag if wanted) and if the result is greater than 0xF we
 * know a borrow out from the first 4 bits occurred.
 */
static void _update_flag_ac_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool sub_borrow) {
    int borrow = sub_borrow ? cpu_get_flag_bit(cpu, CY) : 0;
    val1 &= 0xF;
    val2 = ~((val2 & 0xF) + borrow) + 1;
    cpu_set_flag_bit(cpu, AC, val1 + val2 > 0xF);
}

/* Simply sets the CY and AC flags low. */
static void _clear_cy_ac(CPU *cpu) {
    cpu_set_flag_bit(cpu, CY, false);
    cpu_set_flag_bit(cpu, AC, false);
}

/* Fortunately it seems these flags are updated the same way throughout all
 * instructions so these should be safe to call from any instruction that
 * affects that flag.
 */

/* Sets sign flag equal to value of most-significant bit (bit 7) of res. */
static void _update_flag_s(CPU *cpu, uint8_t res) {
    cpu_set_flag_bit(cpu, S, res & (1 << 7));
}

/* Sets zero flag equal to 1 if res == 0, otherwise set to 0.  */
static void _update_flag_z(CPU *cpu, uint8_t res) {
    cpu_set_flag_bit(cpu, Z, !res);
}

/* Sets parity flag to 1 if number of 1 bits in res is even, 0 otherwise. */
static void _update_flag_p(CPU *cpu, uint8_t res) {
    int high_bits = 0;
    while (res) {
        high_bits += res & 1;
        res >>= 1;
    }

    cpu_set_flag_bit(cpu, P, !(high_bits % 2));
}


/*** PUBLIC FUNCTIONS ***/
void cpu_init(CPU *cpu) {
    _init_opcode_size();
    cpu_reset(cpu);
}

void cpu_reset(CPU *cpu) {
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

    // Clear memory (mainly useful for tests)
    for (int i = 0; i < MAX_MEM; i++) {
        cpu->memory[i] = 0;
    }

    // Reset any helper variables
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

bool cpu_get_flag_bit(const CPU *cpu, FLAG_BITS flag) {
    return cpu->reg[FLAGS] & flag;
}

void cpu_set_flag_bit(CPU *cpu, FLAG_BITS flag, bool high) {
    if (high) {
        cpu->reg[FLAGS] |= flag;
    } else {
        cpu->reg[FLAGS] &= ~flag;
    }
}

uint16_t cpu_get_reg_pair(const CPU *cpu, REGISTERS reg1, REGISTERS reg2) {
    return (cpu->reg[reg1] << 8) | cpu->reg[reg2];
}

void cpu_set_reg_pair(CPU *cpu, REGISTERS reg1, REGISTERS reg2, uint16_t val) {
    cpu->reg[reg1] = val >> 8;
    cpu->reg[reg2] = val & 0xFF;
}

void cpu_tick(CPU *cpu) {
    uint8_t opcode = cpu->memory[cpu->pc];

    /* In case we run into an opcode that expects 1-2 bytes as "operands"
     * (Such as a 2-byte address)
     * It is important to keep in mind that often, if the 2 operands represent a
     * 16-bit value such as an address, the first byte actually represents the
     * LSB of the 16-bit value and the second byte the MSB due to the
     * little-endianess of the 8080.
     */
    uint8_t operands[2] = {
        cpu->memory[cpu->pc + 1],
        cpu->memory[cpu->pc + 2]
    };

    /* Manual states PC is incremented before execution. Order is important
     * because some opcodes may change the PC.
     */
    cpu->pc += opcode_size[opcode];

    /* Instructions are ordered by their instruction group and by the order in
     * which they appear in the Assembly Manual.
     */
    switch (opcode) {
    /* Data Transfer Group */
    case MOV_A_A:
        // Do stuff
        break;
    case MOV_A_B:
        // Do stuff
        break;
    case MOV_A_C:
        // Do stuff
        break;
    case MOV_A_D:
        // Do stuff
        break;
    case MOV_A_E:
        // Do stuff
        break;
    case MOV_A_H:
        // Do stuff
        break;
    case MOV_A_L:
        // Do stuff
        break;
    case MOV_A_M:
        // Do stuff
        break;
    case MOV_B_A:
        // Do stuff
        break;
    case MOV_B_B:
        // Do stuff
        break;
    case MOV_B_C:
        // Do stuff
        break;
    case MOV_B_D:
        // Do stuff
        break;
    case MOV_B_E:
        // Do stuff
        break;
    case MOV_B_H:
        // Do stuff
        break;
    case MOV_B_L:
        // Do stuff
        break;
    case MOV_B_M:
        // Do stuff
        break;
    case MOV_C_A:
        // Do stuff
        break;
    case MOV_C_B:
        // Do stuff
        break;
    case MOV_C_C:
        // Do stuff
        break;
    case MOV_C_D:
        // Do stuff
        break;
    case MOV_C_E:
        // Do stuff
        break;
    case MOV_C_H:
        // Do stuff
        break;
    case MOV_C_L:
        // Do stuff
        break;
    case MOV_C_M:
        // Do stuff
        break;
    case MOV_D_A:
        // Do stuff
        break;
    case MOV_D_B:
        // Do stuff
        break;
    case MOV_D_C:
        // Do stuff
        break;
    case MOV_D_D:
        // Do stuff
        break;
    case MOV_D_E:
        // Do stuff
        break;
    case MOV_D_H:
        // Do stuff
        break;
    case MOV_D_L:
        // Do stuff
        break;
    case MOV_D_M:
        // Do stuff
        break;
    case MOV_E_A:
        // Do stuff
        break;
    case MOV_E_B:
        // Do stuff
        break;
    case MOV_E_C:
        // Do stuff
        break;
    case MOV_E_D:
        // Do stuff
        break;
    case MOV_E_E:
        // Do stuff
        break;
    case MOV_E_H:
        // Do stuff
        break;
    case MOV_E_L:
        // Do stuff
        break;
    case MOV_E_M:
        // Do stuff
        break;
    case MOV_H_A:
        // Do stuff
        break;
    case MOV_H_B:
        // Do stuff
        break;
    case MOV_H_C:
        // Do stuff
        break;
    case MOV_H_D:
        // Do stuff
        break;
    case MOV_H_E:
        // Do stuff
        break;
    case MOV_H_H:
        // Do stuff
        break;
    case MOV_H_L:
        // Do stuff
        break;
    case MOV_H_M:
        // Do stuff
        break;
    case MOV_L_A:
        // Do stuff
        break;
    case MOV_L_B:
        // Do stuff
        break;
    case MOV_L_C:
        // Do stuff
        break;
    case MOV_L_D:
        // Do stuff
        break;
    case MOV_L_E:
        // Do stuff
        break;
    case MOV_L_H:
        // Do stuff
        break;
    case MOV_L_L:
        // Do stuff
        break;
    case MOV_L_M:
        // Do stuff
        break;
    case MOV_M_A:
        // Do stuff
        break;
    case MOV_M_B:
        // Do stuff
        break;
    case MOV_M_C:
        // Do stuff
        break;
    case MOV_M_D:
        // Do stuff
        break;
    case MOV_M_E:
        // Do stuff
        break;
    case MOV_M_H:
        // Do stuff
        break;
    case MOV_M_L:
        // Do stuff
        break;
    case MVI_A:
        // Do stuff
        break;
    case MVI_B:
        // Do stuff
        break;
    case MVI_C:
        // Do stuff
        break;
    case MVI_D:
        // Do stuff
        break;
    case MVI_E:
        // Do stuff
        break;
    case MVI_H:
        // Do stuff
        break;
    case MVI_L:
        // Do stuff
        break;
    case MVI_M:
        // Do stuff
        break;
    case LXI_B:
        // Do stuff
        break;
    case LXI_D:
        // Do stuff
        break;
    case LXI_H:
        // Do stuff
        break;
    case LXI_SP:
        // Do stuff
        break;
    case LDA:
        // Do stuff
        break;
    case STA:
        // Do stuff
        break;
    case LHLD:
        // Do stuff
        break;
    case SHLD:
        // Do stuff
        break;
    case LDAX_B:
        // Do stuff
        break;
    case LDAX_D:
        // Do stuff
        break;
    case STAX_B:
        // Do stuff
        break;
    case STAX_D:
        // Do stuff
        break;
    case XCHG:
        // Do stuff
        break;
    
    /* Arithmetic Group */
    case ADD_A:
        // Do stuff
        break;
    case ADD_B:
        // Do stuff
        break;
    case ADD_C:
        // Do stuff
        break;
    case ADD_D:
        // Do stuff
        break;
    case ADD_E:
        // Do stuff
        break;
    case ADD_H:
        // Do stuff
        break;
    case ADD_L:
        // Do stuff
        break;
    case ADD_M:
        // Do stuff
        break;
    case ADI:
        // Do stuff
        break;
    case ADC_A:
        // Do stuff
        break;
    case ADC_B:
        // Do stuff
        break;
    case ADC_C:
        // Do stuff
        break;
    case ADC_D:
        // Do stuff
        break;
    case ADC_E:
        // Do stuff
        break;
    case ADC_H:
        // Do stuff
        break;
    case ADC_L:
        // Do stuff
        break;
    case ADC_M:
        // Do stuff
        break;
    case ACI:
        // Do stuff
        break;
    case SUB_A:
        // Do stuff
        break;
    case SUB_B:
        // Do stuff
        break;
    case SUB_C:
        // Do stuff
        break;
    case SUB_D:
        // Do stuff
        break;
    case SUB_E:
        // Do stuff
        break;
    case SUB_H:
        // Do stuff
        break;
    case SUB_L:
        // Do stuff
        break;
    case SUB_M:
        // Do stuff
        break;
    case SUI:
        // Do stuff
        break;
    case SBB_A:
        // Do stuff
        break;
    case SBB_B:
        // Do stuff
        break;
    case SBB_C:
        // Do stuff
        break;
    case SBB_D:
        // Do stuff
        break;
    case SBB_E:
        // Do stuff
        break;
    case SBB_H:
        // Do stuff
        break;
    case SBB_L:
        // Do stuff
        break;
    case SBB_M:
        // Do stuff
        break;
    case SBI:
        // Do stuff
        break;
    case INR_A:
        // Do stuff
        break;
    case INR_B:
        // Do stuff
        break;
    case INR_C:
        // Do stuff
        break;
    case INR_D:
        // Do stuff
        break;
    case INR_E:
        // Do stuff
        break;
    case INR_H:
        // Do stuff
        break;
    case INR_L:
        // Do stuff
        break;
    case INR_M:
        // Do stuff
        break;
    case DCR_A:
        // Do stuff
        break;
    case DCR_B:
        // Do stuff
        break;
    case DCR_C:
        // Do stuff
        break;
    case DCR_D:
        // Do stuff
        break;
    case DCR_E:
        // Do stuff
        break;
    case DCR_H:
        // Do stuff
        break;
    case DCR_L:
        // Do stuff
        break;
    case DCR_M:
        // Do stuff
        break;
    case INX_B:
        // Do stuff
        break;
    case INX_D:
        // Do stuff
        break;
    case INX_H:
        // Do stuff
        break;
    case INX_SP:
        // Do stuff
        break;
    case DCX_B:
        // Do stuff
        break;
    case DCX_D:
        // Do stuff
        break;
    case DCX_H:
        // Do stuff
        break;
    case DCX_SP:
        // Do stuff
        break;
    case DAD_B:
        // Do stuff
        break;
    case DAD_D:
        // Do stuff
        break;
    case DAD_H:
        // Do stuff
        break;
    case DAD_SP:
        // Do stuff
        break;
    case DAA:
        // Do stuff
        break;

    /* Logical Group */
    case ANA_A:
        // Do stuff
        break;
    case ANA_B:
        // Do stuff
        break;
    case ANA_C:
        // Do stuff
        break;
    case ANA_D:
        // Do stuff
        break;
    case ANA_E:
        // Do stuff
        break;
    case ANA_H:
        // Do stuff
        break;
    case ANA_L:
        // Do stuff
        break;
    case ANA_M:
        // Do stuff
        break;
    case ANI:
        // Do stuff
        break;
    case XRA_A:
        // Do stuff
        break;
    case XRA_B:
        // Do stuff
        break;
    case XRA_C:
        // Do stuff
        break;
    case XRA_D:
        // Do stuff
        break;
    case XRA_E:
        // Do stuff
        break;
    case XRA_H:
        // Do stuff
        break;
    case XRA_L:
        // Do stuff
        break;
    case XRA_M:
        // Do stuff
        break;
    case XRI:
        // Do stuff
        break;
    case ORA_A:
        // Do stuff
        break;
    case ORA_B:
        // Do stuff
        break;
    case ORA_C:
        // Do stuff
        break;
    case ORA_D:
        // Do stuff
        break;
    case ORA_E:
        // Do stuff
        break;
    case ORA_H:
        // Do stuff
        break;
    case ORA_L:
        // Do stuff
        break;
    case ORA_M:
        // Do stuff
        break;
    case ORI:
        // Do stuff
        break;
    case CMP_A:
        // Do stuff
        break;
    case CMP_B:
        // Do stuff
        break;
    case CMP_C:
        // Do stuff
        break;
    case CMP_D:
        // Do stuff
        break;
    case CMP_E:
        // Do stuff
        break;
    case CMP_H:
        // Do stuff
        break;
    case CMP_L:
        // Do stuff
        break;
    case CMP_M:
        // Do stuff
        break;
    case CPI:
        // Do stuff
        break;
    case RLC:
        // Do stuff
        break;
    case RRC:
        // Do stuff
        break;
    case RAL:
        // Do stuff
        break;
    case RAR:
        // Do stuff
        break;
    case CMA:
        // Do stuff
        break;
    case CMC:
        // Do stuff
        break;
    case STC:
        // Do stuff
        break;

    /* Branch Group */
    case JMP:
        // Do stuff
        break;
    case JNZ:
        // Do stuff
        break;
    case JZ:
        // Do stuff
        break;
    case JNC:
        // Do stuff
        break;
    case JC:
        // Do stuff
        break;
    case JPO:
        // Do stuff
        break;
    case JPE:
        // Do stuff
        break;
    case JP:
        // Do stuff
        break;
    case JM:
        // Do stuff
        break;
    case CALL:
        // Do stuff
        break;
    case CNZ:
        // Do stuff
        break;
    case CZ:
        // Do stuff
        break;
    case CNC:
        // Do stuff
        break;
    case CC:
        // Do stuff
        break;
    case CPO:
        // Do stuff
        break;
    case CPE:
        // Do stuff
        break;
    case CP:
        // Do stuff
        break;
    case CM:
        // Do stuff
        break;
    case RET:
        // Do stuff
        break;
    case RNZ:
        // Do stuff
        break;
    case RZ:
        // Do stuff
        break;
    case RNC:
        // Do stuff
        break;
    case RC:
        // Do stuff
        break;
    case RPO:
        // Do stuff
        break;
    case RPE:
        // Do stuff
        break;
    case RP:
        // Do stuff
        break;
    case RM:
        // Do stuff
        break;
    case RST_0:
        // Do stuff
        break;
    case RST_1:
        // Do stuff
        break;
    case RST_2:
        // Do stuff
        break;
    case RST_3:
        // Do stuff
        break;
    case RST_4:
        // Do stuff
        break;
    case RST_5:
        // Do stuff
        break;
    case RST_6:
        // Do stuff
        break;
    case RST_7:
        // Do stuff
        break;
    case PCHL:
        // Do stuff
        break;
    
    /* Stack/IO Group */
    case PUSH_B:
        // Do stuff
        break;
    case PUSH_D:
        // Do stuff
        break;
    case PUSH_H:
        // Do stuff
        break;
    case PUSH_PSW:
        // Do stuff
        break;
    case POP_B:
        // Do stuff
        break;
    case POP_D:
        // Do stuff
        break;
    case POP_H:
        // Do stuff
        break;
    case POP_PSW:
        // Do stuff
        break;
    case XTHL:
        // Do stuff
        break;
    case SPHL:
        // Do stuff
        break;
    case IN:
        // Do stuff
        break;
    case OUT:
        // Do stuff
        break;
    case EI:
        // Do stuff
        break;
    case DI:
        // Do stuff
        break;
    case HLT:
        // Do stuff
        break;
    case NOP:
        printf("NOP!\n");
        break;
    case EXIT:
        cpu->exit = true;
        break;

    default:
       fprintf(stderr, "Encountered undefined opcode.\n");
       break;
    }
}
