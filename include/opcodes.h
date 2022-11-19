#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"

void MOV_R_R(CPU *cpu, REGISTERS dest, REGISTERS src);
void MOV_R_M(CPU *cpu, REGISTERS dest);
void MOV_M_R(CPU *cpu, REGISTERS src);
void MVI_R(CPU *cpu, REGISTERS dest, uint8_t operand);
void MVI_M(CPU *cpu, uint8_t operand);

#endif
