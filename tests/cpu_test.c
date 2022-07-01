#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define TEST(opcode, condition) \
{ \
    if (!(condition)) { \
        fprintf(stderr, "%s: Test `"#condition"` failed on line %d.\n", \
                opcode_str[opcode], __LINE__); \
        exit(1); \
    } \
}

// Making this global just keeps down on clutter
CPU cpu;

/* Resets the CPU and loads the opcode into beginning of memory. */
void setup_test(OPCODES opcode) {
    cpu_reset(&cpu);
    cpu.memory[0] = opcode;
}

/* Test for a register-to-register move. */
void test_MOV_R_R(OPCODES opcode, REGISTERS dest, REGISTERS src) {
    setup_test(opcode);
    cpu.reg[src] = 0x42;
    cpu_tick(&cpu);

    TEST(opcode, cpu.reg[dest] == 0x42);
}

/* Test for a memory-to-register move. */
void test_MOV_R_M(OPCODES opcode, REGISTERS dest) {
    setup_test(opcode);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    TEST(opcode, cpu.reg[dest] == 0x42);
}

/* Test for a register-to-memory move. */
void test_MOV_M_R(OPCODES opcode, REGISTERS src) {
    setup_test(opcode);
    cpu.reg[src] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    TEST(opcode, cpu.memory[0xDEAD] == 0x42);
}

/* MOV M, H and MOV M, L might be special cases so give their own tests. */
void test_MOV_M_H(void) {
    setup_test(MOV_M_H);
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    // Strange opcode...
    TEST(MOV_M_H, cpu.memory[0xDEAD] == 0xDE);
}

void test_MOV_M_L(void) {
    setup_test(MOV_M_L);
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    // Also strange opcode...
    TEST(MOV_M_L, cpu.memory[0xDEAD] == 0xAD);
}

/* Test for an immediate-to-register move. */
void test_MVI_R(OPCODES opcode, REGISTERS dest) {
    setup_test(opcode);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    TEST(opcode, cpu.reg[dest] == 0x42);
}

/* Test for an immediate-to-memory move. */
void test_MVI_M(void) {
    setup_test(MVI_M);
    cpu.memory[1] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    TEST(MVI_M, cpu.memory[cpu_get_reg_pair(&cpu, H, L)] == 0x42);
}

/* Test for immediate-to-register pair load. */
void test_LXI_RP(OPCODES opcode, REGISTERS r1, REGISTERS r2) {
    setup_test(opcode);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu_tick(&cpu);

    TEST(opcode, cpu_get_reg_pair(&cpu, r1, r2) == 0xDEAD);
}

/* Test for immediate-to-stack pointer load. */
void test_LXI_SP(void) {
    setup_test(LXI_SP);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu_tick(&cpu);

    TEST(LXI_SP, cpu.sp == 0xDEAD);
}

/* Test for memory-to-accumulator load. */
void test_LDA(void) {
    setup_test(LDA);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.memory[0xDEAD] = 0x42;
    cpu_tick(&cpu);

    TEST(LDA, cpu.reg[A] == 0x42);
}

/* Test for accumulator-to-memory move. */
void test_STA(void) {
    setup_test(STA);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    TEST(STA, cpu.memory[0xDEAD] == 0x42);
}

/* Test for memory-to-(HL) load. */
void test_LHLD(void) {
    setup_test(LHLD);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.memory[0xDEAD] = 0x42;
    cpu.memory[0xDEAE] = 0x69;
    cpu_tick(&cpu);

    TEST(LHLD, cpu.reg[L] == 0x42);
    TEST(LHLD, cpu.reg[H] == 0x69);
}

/* Test for (HL)-to-memory move. */
void test_SHLD(void) {
    setup_test(SHLD);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.reg[L] = 0x42;
    cpu.reg[H] = 0x69;
    cpu_tick(&cpu);

    TEST(SHLD, cpu.memory[0xDEAD] == 0x42);
    TEST(SHLD, cpu.memory[0xDEAE] == 0x69);
}

/* Test for memory-to-accumulator indirect load. */
void test_LDAX_RP(OPCODES opcode, REGISTERS r1, REGISTERS r2) {
    setup_test(opcode);
    cpu.reg[r1] = 0xDE;
    cpu.reg[r2] = 0xAD;
    cpu.memory[0xDEAD] = 0x42;
    cpu_tick(&cpu);

    TEST(opcode, cpu.reg[A] == 0x42);
}

/* Test for accumulator-to-memory indirect move. */
void test_STAX_RP(OPCODES opcode, REGISTERS r1, REGISTERS r2) {
    setup_test(opcode);
    cpu.reg[r1] = 0xDE;
    cpu.reg[r2] = 0xAD;
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    TEST(opcode, cpu.memory[0xDEAD] == 0x42);
}

/* Test for H <-> L and D <-> E exchange. */
void test_XCHG(void) {
    setup_test(XCHG);
    cpu.reg[H] = 0xDE;
    cpu.reg[L] = 0xAD;
    cpu.reg[D] = 0xBE;
    cpu.reg[E] = 0xEF;
    cpu_tick(&cpu);

    TEST(XCHG, cpu.reg[H] == 0xBE);
    TEST(XCHG, cpu.reg[L] == 0xEF);
    TEST(XCHG, cpu.reg[D] == 0xDE);
    TEST(XCHG, cpu.reg[E] == 0xAD);
}

int main(void) {
    cpu_init(&cpu);

    /* Data Transfer Tests */
    test_MOV_R_R(MOV_A_A, A, A);
    test_MOV_R_R(MOV_A_B, A, B);
    test_MOV_R_R(MOV_A_C, A, C);
    test_MOV_R_R(MOV_A_D, A, D);
    test_MOV_R_R(MOV_A_E, A, E);
    test_MOV_R_R(MOV_A_H, A, H);
    test_MOV_R_R(MOV_A_L, A, L);

    test_MOV_R_R(MOV_B_A, B, A);
    test_MOV_R_R(MOV_B_B, B, B);
    test_MOV_R_R(MOV_B_C, B, C);
    test_MOV_R_R(MOV_B_D, B, D);
    test_MOV_R_R(MOV_B_E, B, E);
    test_MOV_R_R(MOV_B_H, B, H);
    test_MOV_R_R(MOV_B_L, B, L);

    test_MOV_R_R(MOV_C_A, C, A);
    test_MOV_R_R(MOV_C_B, C, B);
    test_MOV_R_R(MOV_C_C, C, C);
    test_MOV_R_R(MOV_C_D, C, D);
    test_MOV_R_R(MOV_C_E, C, E);
    test_MOV_R_R(MOV_C_H, C, H);
    test_MOV_R_R(MOV_C_L, C, L);

    test_MOV_R_R(MOV_D_A, D, A);
    test_MOV_R_R(MOV_D_B, D, B);
    test_MOV_R_R(MOV_D_C, D, C);
    test_MOV_R_R(MOV_D_D, D, D);
    test_MOV_R_R(MOV_D_E, D, E);
    test_MOV_R_R(MOV_D_H, D, H);
    test_MOV_R_R(MOV_D_L, D, L);

    test_MOV_R_R(MOV_E_A, E, A);
    test_MOV_R_R(MOV_E_B, E, B);
    test_MOV_R_R(MOV_E_C, E, C);
    test_MOV_R_R(MOV_E_D, E, D);
    test_MOV_R_R(MOV_E_E, E, E);
    test_MOV_R_R(MOV_E_H, E, H);
    test_MOV_R_R(MOV_E_L, E, L);

    test_MOV_R_R(MOV_H_A, H, A);
    test_MOV_R_R(MOV_H_B, H, B);
    test_MOV_R_R(MOV_H_C, H, C);
    test_MOV_R_R(MOV_H_D, H, D);
    test_MOV_R_R(MOV_H_E, H, E);
    test_MOV_R_R(MOV_H_H, H, H);
    test_MOV_R_R(MOV_H_L, H, L);

    test_MOV_R_R(MOV_L_A, L, A);
    test_MOV_R_R(MOV_L_B, L, B);
    test_MOV_R_R(MOV_L_C, L, C);
    test_MOV_R_R(MOV_L_D, L, D);
    test_MOV_R_R(MOV_L_E, L, E);
    test_MOV_R_R(MOV_L_H, L, H);
    test_MOV_R_R(MOV_L_L, L, L);

    test_MOV_R_M(MOV_A_M, A);
    test_MOV_R_M(MOV_B_M, B);
    test_MOV_R_M(MOV_C_M, C);
    test_MOV_R_M(MOV_D_M, D);
    test_MOV_R_M(MOV_E_M, E);
    test_MOV_R_M(MOV_H_M, H);
    test_MOV_R_M(MOV_L_M, L);

    test_MOV_M_R(MOV_M_A, A);
    test_MOV_M_R(MOV_M_B, B);
    test_MOV_M_R(MOV_M_C, C);
    test_MOV_M_R(MOV_M_D, D);
    test_MOV_M_R(MOV_M_E, E);
    test_MOV_M_H();
    test_MOV_M_L();

    test_MVI_R(MVI_A, A);
    test_MVI_R(MVI_B, B);
    test_MVI_R(MVI_C, C);
    test_MVI_R(MVI_D, D);
    test_MVI_R(MVI_E, E);
    test_MVI_R(MVI_H, H);
    test_MVI_R(MVI_L, L);
    test_MVI_M();

    test_LXI_RP(LXI_B, B, C);
    test_LXI_RP(LXI_D, D, E);
    test_LXI_RP(LXI_H, H, L);
    test_LXI_SP();

    test_LDA();
    test_STA();
    test_LHLD();
    test_SHLD();

    test_LDAX_RP(LDAX_B, B, C);
    test_LDAX_RP(LDAX_D, D, E);

    test_STAX_RP(STAX_B, B, C);
    test_STAX_RP(STAX_D, D, E);

    test_XCHG();

    /* Arithmetic Tests */

    /* Logical Tests */

    /* Branch Tests */

    /* Stack/IO Tests */

    // If we got here, a test never failed!
    printf("All tests passed!\n");
}
