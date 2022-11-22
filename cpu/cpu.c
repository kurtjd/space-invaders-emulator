#include <stdio.h>
#include "cpu.h"
#include "opcodes.h"

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

const int opcode_cycles[NUM_OPCODES] = {
    4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4,

    4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4,

    4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4,

    4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4,

    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,

    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,

    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,

    7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5,

    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

    5, 10, 10, 10, 11, 11, 7, 11, 5, 10, 10, 10, 11, 17, 7, 11,

    5, 10, 10, 10, 11, 11, 7, 11, 5, 10, 10, 10, 11, 17, 7, 11,

    5, 10, 10, 18, 11, 11, 7, 11, 5, 5, 10, 4, 11, 17, 7, 11,

    5, 10, 10, 4, 11, 11, 7, 11, 5, 5, 10, 4, 11, 17, 7, 11
};

int opcode_size[NUM_OPCODES];


/*** PRIVATE FUNCTIONS ***/
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
    opcode_size[OP_MVI_B] = 2;
    opcode_size[OP_MVI_C] = 2;
    opcode_size[OP_MVI_D] = 2;
    opcode_size[OP_MVI_E] = 2;
    opcode_size[OP_MVI_H] = 2;
    opcode_size[OP_MVI_L] = 2;
    opcode_size[OP_MVI_M] = 2;
    opcode_size[OP_MVI_A] = 2;
    opcode_size[OP_ADI]   = 2;
    opcode_size[OP_ACI]   = 2;
    opcode_size[OP_OUT]   = 2;
    opcode_size[OP_SUI]   = 2;
    opcode_size[OP_IN]    = 2;
    opcode_size[OP_SBI]   = 2;
    opcode_size[OP_ANI]   = 2;
    opcode_size[OP_XRI]   = 2;
    opcode_size[OP_ORI]   = 2;
    opcode_size[OP_CPI]   = 2;

    // Explicitly initialize any opcodes with size 3
    opcode_size[OP_LXI_B]  = 3;
    opcode_size[OP_LXI_D]  = 3;
    opcode_size[OP_LXI_H]  = 3;
    opcode_size[OP_SHLD]   = 3;
    opcode_size[OP_LHLD]   = 3;
    opcode_size[OP_LXI_SP] = 3;
    opcode_size[OP_STA]    = 3;
    opcode_size[OP_LDA]    = 3;
    opcode_size[OP_JNZ]    = 3;
    opcode_size[OP_JMP]    = 3;
    opcode_size[OP_CNZ]    = 3;
    opcode_size[OP_JZ]     = 3;
    opcode_size[OP_CZ]     = 3;
    opcode_size[OP_CALL]   = 3;
    opcode_size[OP_JNC]    = 3;
    opcode_size[OP_CNC]    = 3;
    opcode_size[OP_JC]     = 3;
    opcode_size[OP_CC]     = 3;
    opcode_size[OP_JPO]    = 3;
    opcode_size[OP_CPO]    = 3;
    opcode_size[OP_JPE]    = 3;
    opcode_size[OP_CPE]    = 3;
    opcode_size[OP_JP]     = 3;
    opcode_size[OP_CP]     = 3;
    opcode_size[OP_JM]     = 3;
    opcode_size[OP_CM]     = 3;
}

static uint8_t _get_interrupt(int int_num) {
    switch (int_num) {
    case 0: return OP_RST_0;
    case 1: return OP_RST_1;
    case 2: return OP_RST_2;
    case 3: return OP_RST_3;
    case 4: return OP_RST_4;
    case 5: return OP_RST_5;
    case 6: return OP_RST_6;
    case 7: return OP_RST_7;
    default: return OP_RST_0;
    }
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
    cpu->halt = false;
    cpu->total_cycles = 0;
    cpu->instr_complete = true;
    cpu->interrupt = -1;
}

bool cpu_load_rom(CPU *cpu, const char *filename, uint16_t start_addr) {
    FILE *rom = fopen(filename, "rb");

    if (rom) {
        fread(cpu->memory + start_addr, MAX_MEM, 1, rom);
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

uint8_t cpu_get_sw(const CPU *cpu) {
    return (cpu_get_flag_bit(cpu, S) << 7)
         | (cpu_get_flag_bit(cpu, Z) << 6)
         | (cpu_get_flag_bit(cpu, AC) << 4)
         | (cpu_get_flag_bit(cpu, P) << 2)
         | (1 << 1)
         | (cpu_get_flag_bit(cpu, CY));
}

void cpu_req_interrupt(CPU *cpu, uint8_t int_num) {
    cpu->interrupt = int_num;
}

void cpu_print_debug(const CPU *cpu) {
    printf("PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, CYC: %lu (%s)\n",
          cpu->pc,
          (cpu->reg[A] << 8) | cpu_get_sw(cpu),
          (cpu->reg[B] << 8) | cpu->reg[C],
          (cpu->reg[D] << 8) | cpu->reg[E],
          (cpu->reg[H] << 8) | cpu->reg[L],
          cpu->sp,
          cpu->total_cycles,
          opcode_str[cpu->memory[cpu->pc]]);
}

void cpu_tick(CPU *cpu) {
    if (cpu->instr_complete) {
        uint8_t opcode;

        /* In case we run into an opcode that expects 1-2 bytes as "operands"
        * (Such as a 2-byte address)
        * It is important to keep in mind that often, if the 2 operands represent a
        * 16-bit value such as an address, the first byte actually represents the
        * LSB of the 16-bit value and the second byte the MSB due to the
        * little-endianess of the 8080.
        */
        uint8_t operands[MAX_OPERANDS] = {
            cpu->memory[cpu->pc + 1],
            cpu->memory[cpu->pc + 2]
        };

        if (cpu->interrupt >= 0 && cpu_get_flag_bit(cpu, I)) {
            opcode = _get_interrupt(cpu->interrupt);
            cpu->interrupt = -1;
            cpu_set_flag_bit(cpu, I, false);
        } else {
            opcode = cpu->memory[cpu->pc];

            /* Manual states PC is incremented before execution. Order is important
            * because some opcodes may change the PC.
            */
            cpu->pc += opcode_size[opcode];
        }

        cpu->instr_cycles = opcode_cycles[opcode];

        /* Instructions are ordered by their instruction group and by the order in
        * which they appear in the Assembly Manual.
        */
        switch (opcode) {
        /* Data Transfer Group */
        case OP_MOV_A_A:    MOV_R_R(cpu, A, A);           break;
        case OP_MOV_A_B:    MOV_R_R(cpu, A, B);           break;
        case OP_MOV_A_C:    MOV_R_R(cpu, A, C);           break;
        case OP_MOV_A_D:    MOV_R_R(cpu, A, D);           break;
        case OP_MOV_A_E:    MOV_R_R(cpu, A, E);           break;
        case OP_MOV_A_H:    MOV_R_R(cpu, A, H);           break;
        case OP_MOV_A_L:    MOV_R_R(cpu, A, L);           break;
        case OP_MOV_A_M:    MOV_R_M(cpu, A);              break;
        case OP_MOV_B_A:    MOV_R_R(cpu, B, A);           break;
        case OP_MOV_B_B:    MOV_R_R(cpu, B, B);           break;
        case OP_MOV_B_C:    MOV_R_R(cpu, B, C);           break;
        case OP_MOV_B_D:    MOV_R_R(cpu, B, D);           break;
        case OP_MOV_B_E:    MOV_R_R(cpu, B, E);           break;
        case OP_MOV_B_H:    MOV_R_R(cpu, B, H);           break;
        case OP_MOV_B_L:    MOV_R_R(cpu, B, L);           break;
        case OP_MOV_B_M:    MOV_R_M(cpu, B);              break;
        case OP_MOV_C_A:    MOV_R_R(cpu, C, A);           break;
        case OP_MOV_C_B:    MOV_R_R(cpu, C, B);           break;
        case OP_MOV_C_C:    MOV_R_R(cpu, C, C);           break;
        case OP_MOV_C_D:    MOV_R_R(cpu, C, D);           break;
        case OP_MOV_C_E:    MOV_R_R(cpu, C, E);           break;
        case OP_MOV_C_H:    MOV_R_R(cpu, C, H);           break;
        case OP_MOV_C_L:    MOV_R_R(cpu, C, L);           break;
        case OP_MOV_C_M:    MOV_R_M(cpu, C);              break;
        case OP_MOV_D_A:    MOV_R_R(cpu, D, A);           break;
        case OP_MOV_D_B:    MOV_R_R(cpu, D, B);           break;
        case OP_MOV_D_C:    MOV_R_R(cpu, D, C);           break;
        case OP_MOV_D_D:    MOV_R_R(cpu, D, D);           break;
        case OP_MOV_D_E:    MOV_R_R(cpu, D, E);           break;
        case OP_MOV_D_H:    MOV_R_R(cpu, D, H);           break;
        case OP_MOV_D_L:    MOV_R_R(cpu, D, L);           break;
        case OP_MOV_D_M:    MOV_R_M(cpu, D);              break;
        case OP_MOV_E_A:    MOV_R_R(cpu, E, A);           break;
        case OP_MOV_E_B:    MOV_R_R(cpu, E, B);           break;
        case OP_MOV_E_C:    MOV_R_R(cpu, E, C);           break;
        case OP_MOV_E_D:    MOV_R_R(cpu, E, D);           break;
        case OP_MOV_E_E:    MOV_R_R(cpu, E, E);           break;
        case OP_MOV_E_H:    MOV_R_R(cpu, E, H);           break;
        case OP_MOV_E_L:    MOV_R_R(cpu, E, L);           break;
        case OP_MOV_E_M:    MOV_R_M(cpu, E);              break;
        case OP_MOV_H_A:    MOV_R_R(cpu, H, A);           break;
        case OP_MOV_H_B:    MOV_R_R(cpu, H, B);           break;
        case OP_MOV_H_C:    MOV_R_R(cpu, H, C);           break;
        case OP_MOV_H_D:    MOV_R_R(cpu, H, D);           break;
        case OP_MOV_H_E:    MOV_R_R(cpu, H, E);           break;
        case OP_MOV_H_H:    MOV_R_R(cpu, H, H);           break;
        case OP_MOV_H_L:    MOV_R_R(cpu, H, L);           break;
        case OP_MOV_H_M:    MOV_R_M(cpu, H);              break;
        case OP_MOV_L_A:    MOV_R_R(cpu, L, A);           break;
        case OP_MOV_L_B:    MOV_R_R(cpu, L, B);           break;
        case OP_MOV_L_C:    MOV_R_R(cpu, L, C);           break;
        case OP_MOV_L_D:    MOV_R_R(cpu, L, D);           break;
        case OP_MOV_L_E:    MOV_R_R(cpu, L, E);           break;
        case OP_MOV_L_H:    MOV_R_R(cpu, L, H);           break;
        case OP_MOV_L_L:    MOV_R_R(cpu, L, L);           break;
        case OP_MOV_L_M:    MOV_R_M(cpu, L);              break;
        case OP_MOV_M_A:    MOV_M_R(cpu, A);              break;
        case OP_MOV_M_B:    MOV_M_R(cpu, B);              break;
        case OP_MOV_M_C:    MOV_M_R(cpu, C);              break;
        case OP_MOV_M_D:    MOV_M_R(cpu, D);              break;
        case OP_MOV_M_E:    MOV_M_R(cpu, E);              break;
        case OP_MOV_M_H:    MOV_M_R(cpu, H);              break;
        case OP_MOV_M_L:    MOV_M_R(cpu, L);              break;
        case OP_MVI_A:      MVI_R(cpu, A, operands[0]);   break;
        case OP_MVI_B:      MVI_R(cpu, B, operands[0]);   break;
        case OP_MVI_C:      MVI_R(cpu, C, operands[0]);   break;
        case OP_MVI_D:      MVI_R(cpu, D, operands[0]);   break;
        case OP_MVI_E:      MVI_R(cpu, E, operands[0]);   break;
        case OP_MVI_H:      MVI_R(cpu, H, operands[0]);   break;
        case OP_MVI_L:      MVI_R(cpu, L, operands[0]);   break;
        case OP_MVI_M:      MVI_M(cpu, operands[0]);      break;
        case OP_LXI_B:      LXI_RP(cpu, B, operands);     break;
        case OP_LXI_D:      LXI_RP(cpu, D, operands);     break;
        case OP_LXI_H:      LXI_RP(cpu, H, operands);     break;
        case OP_LXI_SP:     LXI_SP(cpu, operands);        break;
        case OP_LDA:        LDA(cpu, operands);           break;
        case OP_STA:        STA(cpu, operands);           break;
        case OP_LHLD:       LHLD(cpu, operands);          break;
        case OP_SHLD:       SHLD(cpu, operands);          break;
        case OP_LDAX_B:     LDAX_RP(cpu, B);              break;
        case OP_LDAX_D:     LDAX_RP(cpu, D);              break;
        case OP_STAX_B:     STAX_RP(cpu, B);              break;
        case OP_STAX_D:     STAX_RP(cpu, D);              break;
        case OP_XCHG:       XCHG(cpu);                    break;
        
        /* Arithmetic Group */
        case OP_ADD_A:      ADD_R(cpu, A);                break;
        case OP_ADD_B:      ADD_R(cpu, B);                break;
        case OP_ADD_C:      ADD_R(cpu, C);                break;
        case OP_ADD_D:      ADD_R(cpu, D);                break;
        case OP_ADD_E:      ADD_R(cpu, E);                break;
        case OP_ADD_H:      ADD_R(cpu, H);                break;
        case OP_ADD_L:      ADD_R(cpu, L);                break;
        case OP_ADD_M:      ADD_M(cpu);                   break;
        case OP_ADI:        ADI(cpu, operands[0]);        break;
        case OP_ADC_A:      ADC_R(cpu, A);                break;
        case OP_ADC_B:      ADC_R(cpu, B);                break;
        case OP_ADC_C:      ADC_R(cpu, C);                break;
        case OP_ADC_D:      ADC_R(cpu, D);                break;
        case OP_ADC_E:      ADC_R(cpu, E);                break;
        case OP_ADC_H:      ADC_R(cpu, H);                break;
        case OP_ADC_L:      ADC_R(cpu, L);                break;
        case OP_ADC_M:      ADC_M(cpu);                   break;
        case OP_ACI:        ACI(cpu, operands[0]);        break;
        case OP_SUB_A:      SUB_R(cpu, A);                break;
        case OP_SUB_B:      SUB_R(cpu, B);                break;
        case OP_SUB_C:      SUB_R(cpu, C);                break;
        case OP_SUB_D:      SUB_R(cpu, D);                break;
        case OP_SUB_E:      SUB_R(cpu, E);                break;
        case OP_SUB_H:      SUB_R(cpu, H);                break;
        case OP_SUB_L:      SUB_R(cpu, L);                break;
        case OP_SUB_M:      SUB_M(cpu);                   break;
        case OP_SUI:        SUI(cpu, operands[0]);        break;
        case OP_SBB_A:      SBB_R(cpu, A);                break;
        case OP_SBB_B:      SBB_R(cpu, B);                break;
        case OP_SBB_C:      SBB_R(cpu, C);                break;
        case OP_SBB_D:      SBB_R(cpu, D);                break;
        case OP_SBB_E:      SBB_R(cpu, E);                break;
        case OP_SBB_H:      SBB_R(cpu, H);                break;
        case OP_SBB_L:      SBB_R(cpu, L);                break;
        case OP_SBB_M:      SBB_M(cpu);                   break;
        case OP_SBI:        SBI(cpu, operands[0]);        break;
        case OP_INR_A:      INR_R(cpu, A);                break;
        case OP_INR_B:      INR_R(cpu, B);                break;
        case OP_INR_C:      INR_R(cpu, C);                break;
        case OP_INR_D:      INR_R(cpu, D);                break;
        case OP_INR_E:      INR_R(cpu, E);                break;
        case OP_INR_H:      INR_R(cpu, H);                break;
        case OP_INR_L:      INR_R(cpu, L);                break;
        case OP_INR_M:      INR_M(cpu);                   break;
        case OP_DCR_A:      DCR_R(cpu, A);                break;
        case OP_DCR_B:      DCR_R(cpu, B);                break;
        case OP_DCR_C:      DCR_R(cpu, C);                break;
        case OP_DCR_D:      DCR_R(cpu, D);                break;
        case OP_DCR_E:      DCR_R(cpu, E);                break;
        case OP_DCR_H:      DCR_R(cpu, H);                break;
        case OP_DCR_L:      DCR_R(cpu, L);                break;
        case OP_DCR_M:      DCR_M(cpu);                   break;
        case OP_INX_B:      INX_RP(cpu, B);               break;
        case OP_INX_D:      INX_RP(cpu, D);               break;
        case OP_INX_H:      INX_RP(cpu, H);               break;
        case OP_INX_SP:     INX_SP(cpu);                  break;
        case OP_DCX_B:      DCX_RP(cpu, B);               break;
        case OP_DCX_D:      DCX_RP(cpu, D);               break;
        case OP_DCX_H:      DCX_RP(cpu, H);               break;
        case OP_DCX_SP:     DCX_SP(cpu);                  break;
        case OP_DAD_B:      DAD_RP(cpu, B);               break;
        case OP_DAD_D:      DAD_RP(cpu, D);               break;
        case OP_DAD_H:      DAD_RP(cpu, H);               break;
        case OP_DAD_SP:     DAD_SP(cpu);                  break;
        case OP_DAA:        DAA(cpu);                     break;

        /* Logical Group */
        case OP_ANA_A:      ANA_R(cpu, A);                break;
        case OP_ANA_B:      ANA_R(cpu, B);                break;
        case OP_ANA_C:      ANA_R(cpu, C);                break;
        case OP_ANA_D:      ANA_R(cpu, D);                break;
        case OP_ANA_E:      ANA_R(cpu, E);                break;
        case OP_ANA_H:      ANA_R(cpu, H);                break;
        case OP_ANA_L:      ANA_R(cpu, L);                break;
        case OP_ANA_M:      ANA_M(cpu);                   break;
        case OP_ANI:        ANI(cpu, operands[0]);        break;
        case OP_XRA_A:      XRA_R(cpu, A);                break;
        case OP_XRA_B:      XRA_R(cpu, B);                break;
        case OP_XRA_C:      XRA_R(cpu, C);                break;
        case OP_XRA_D:      XRA_R(cpu, D);                break;
        case OP_XRA_E:      XRA_R(cpu, E);                break;
        case OP_XRA_H:      XRA_R(cpu, H);                break;
        case OP_XRA_L:      XRA_R(cpu, L);                break;
        case OP_XRA_M:      XRA_M(cpu);                   break;
        case OP_XRI:        XRI(cpu, operands[0]);        break;
        case OP_ORA_A:      ORA_R(cpu, A);                break;
        case OP_ORA_B:      ORA_R(cpu, B);                break;
        case OP_ORA_C:      ORA_R(cpu, C);                break;
        case OP_ORA_D:      ORA_R(cpu, D);                break;
        case OP_ORA_E:      ORA_R(cpu, E);                break;
        case OP_ORA_H:      ORA_R(cpu, H);                break;
        case OP_ORA_L:      ORA_R(cpu, L);                break;
        case OP_ORA_M:      ORA_M(cpu);                   break;
        case OP_ORI:        ORI(cpu, operands[0]);        break;
        case OP_CMP_A:      CMP_R(cpu, A);                break;
        case OP_CMP_B:      CMP_R(cpu, B);                break;
        case OP_CMP_C:      CMP_R(cpu, C);                break;
        case OP_CMP_D:      CMP_R(cpu, D);                break;
        case OP_CMP_E:      CMP_R(cpu, E);                break;
        case OP_CMP_H:      CMP_R(cpu, H);                break;
        case OP_CMP_L:      CMP_R(cpu, L);                break;
        case OP_CMP_M:      CMP_M(cpu);                   break;
        case OP_CPI:        CPI(cpu, operands[0]);        break;
        case OP_RLC:        RLC(cpu);                     break;
        case OP_RRC:        RRC(cpu);                     break;
        case OP_RAL:        RAL(cpu);                     break;
        case OP_RAR:        RAR(cpu);                     break;
        case OP_CMA:        CMA(cpu);                     break;
        case OP_CMC:        CMC(cpu);                     break;
        case OP_STC:        STC(cpu);                     break;

        /* Branch Group */
        case OP_JMP:        JMP(cpu, operands);           break;
        case OP_JNZ:        JCC(cpu, Z, 0, operands);     break;
        case OP_JZ:         JCC(cpu, Z, 1, operands);     break;
        case OP_JNC:        JCC(cpu, CY, 0, operands);    break;
        case OP_JC:         JCC(cpu, CY, 1, operands);    break;
        case OP_JPO:        JCC(cpu, P, 0, operands);     break;
        case OP_JPE:        JCC(cpu, P, 1, operands);     break;
        case OP_JP:         JCC(cpu, S, 0, operands);     break;
        case OP_JM:         JCC(cpu, S, 1, operands);     break;
        case OP_CALL:       CALL(cpu, operands);          break;
        case OP_CNZ:        CCC(cpu, Z, 0, operands);     break;
        case OP_CZ:         CCC(cpu, Z, 1, operands);     break;
        case OP_CNC:        CCC(cpu, CY, 0, operands);    break;
        case OP_CC:         CCC(cpu, CY, 1, operands);    break;
        case OP_CPO:        CCC(cpu, P, 0, operands);     break;
        case OP_CPE:        CCC(cpu, P, 1, operands);     break;
        case OP_CP:         CCC(cpu, S, 0, operands);     break;
        case OP_CM:         CCC(cpu, S, 1, operands);     break;
        case OP_RET:        RET(cpu);                     break;
        case OP_RNZ:        RCC(cpu, Z, 0);               break;
        case OP_RZ:         RCC(cpu, Z, 1);               break;
        case OP_RNC:        RCC(cpu, CY, 0);              break;
        case OP_RC:         RCC(cpu, CY, 1);              break;
        case OP_RPO:        RCC(cpu, P, 0);               break;
        case OP_RPE:        RCC(cpu, P, 1);               break;
        case OP_RP:         RCC(cpu, S, 0);               break;
        case OP_RM:         RCC(cpu, S, 1);               break;
        case OP_RST_0:      RST_N(cpu, 0);                break;
        case OP_RST_1:      RST_N(cpu, 1);                break;
        case OP_RST_2:      RST_N(cpu, 2);                break;
        case OP_RST_3:      RST_N(cpu, 3);                break;
        case OP_RST_4:      RST_N(cpu, 4);                break;
        case OP_RST_5:      RST_N(cpu, 5);                break;
        case OP_RST_6:      RST_N(cpu, 6);                break;
        case OP_RST_7:      RST_N(cpu, 7);                break;
        case OP_PCHL:       PCHL(cpu);                    break;
        
        /* Stack/IO Group */
        case OP_PUSH_B:     PUSH_RP(cpu, B);              break;
        case OP_PUSH_D:     PUSH_RP(cpu, D);              break;
        case OP_PUSH_H:     PUSH_RP(cpu, H);              break;
        case OP_PUSH_PSW:   PUSH_PSW(cpu);                break;
        case OP_POP_B:      POP_RP(cpu, B);               break;
        case OP_POP_D:      POP_RP(cpu, D);               break;
        case OP_POP_H:      POP_RP(cpu, H);               break;
        case OP_POP_PSW:    POP_PSW(cpu);                 break;
        case OP_XTHL:       XTHL(cpu);                    break;
        case OP_SPHL:       SPHL(cpu);                    break;
        case OP_IN:         IN(cpu, operands[0]);         break;
        case OP_OUT:        OUT(cpu, operands[0]);        break;
        case OP_EI:         EI(cpu);                      break;
        case OP_DI:         DI(cpu);                      break;
        case OP_HLT:        HLT(cpu);                     break;
        case OP_NOP:        NOP();                        break;
        case OP_EXIT:       EXIT(cpu);                    break;

        /* Handle undefined opcode encounters */
        default:            UNDEFINED(cpu);               break;
        }

        cpu->instr_complete = false;
        cpu->cycle_cum = 1;
        cpu->interrupt = -1;
    }

    /* Wait for current instruction to complete before
     * moving to next instruction. */
    if (cpu->cycle_cum < cpu->instr_cycles) {
        cpu->cycle_cum++;
    } else {
        cpu->instr_complete = true;
    }

    cpu->total_cycles++;
}
