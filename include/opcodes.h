#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"

void MOV_R_R(CPU *cpu, REGISTERS dest, REGISTERS src);
void MOV_R_M(CPU *cpu, REGISTERS dest);
void MOV_M_R(CPU *cpu, REGISTERS src);
void MVI_R(CPU *cpu, REGISTERS dest, uint8_t operand);
void MVI_M(CPU *cpu, uint8_t operand);
void LXI_RP(CPU *cpu, REGISTERS dest, const uint8_t operands[2]);
void LXI_SP(CPU *cpu, const uint8_t operands[2]);
void LDA(CPU *cpu, const uint8_t operands[2]);
void STA(CPU *cpu, const uint8_t operands[2]);
void LHLD(CPU *cpu, const uint8_t operands[2]);
void SHLD(CPU *cpu, const uint8_t operands[2]);
void LDAX_RP(CPU *cpu, REGISTERS src);
void STAX_RP(CPU *cpu, REGISTERS src);
void XCHG(CPU *cpu);
void ADD_R(CPU *cpu, REGISTERS src);
void ADD_M(CPU *cpu);

#endif
