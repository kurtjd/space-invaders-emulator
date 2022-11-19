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
void ADI(CPU *cpu, uint8_t operand);
void ADC_R(CPU *cpu, REGISTERS src);
void ADC_M(CPU *cpu);
void ACI(CPU *cpu, uint8_t operand);
void SUB_R(CPU *cpu, REGISTERS src);
void SUB_M(CPU *cpu);
void SUI(CPU *cpu, uint8_t operand);
void SBB_R(CPU *cpu, REGISTERS src);
void SBB_M(CPU *cpu);
void SBI(CPU *cpu, uint8_t operand);
void INR_R(CPU *cpu, REGISTERS dest);
void INR_M(CPU *cpu);
void DCR_R(CPU *cpu, REGISTERS dest);
void DCR_M(CPU *cpu);
void INX_RP(CPU *cpu, REGISTERS dest);
void INX_SP(CPU *cpu);
void DCX_RP(CPU *cpu, REGISTERS dest);
void DCX_SP(CPU *cpu);
void DAD_RP(CPU *cpu, REGISTERS src);
void DAD_SP(CPU *cpu);
void DAA(CPU *cpu);
void ANA_R(CPU *cpu, REGISTERS src);
void ANA_M(CPU *cpu);
void ANI(CPU *cpu, uint8_t operand);
void XRA_R(CPU *cpu, REGISTERS src);
void XRA_M(CPU *cpu);
void XRI(CPU *cpu, uint8_t operand);
void ORA_R(CPU *cpu, REGISTERS src);
void ORA_M(CPU *cpu);
void ORI(CPU *cpu, uint8_t operand);
void CMP_R(CPU *cpu, REGISTERS src);
void CMP_M(CPU *cpu);
void CPI(CPU *cpu, uint8_t operand);

#endif
