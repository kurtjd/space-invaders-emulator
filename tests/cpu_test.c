#undef NDEBUG
#include <stdio.h>
#include <assert.h>
#include "cpu.h"

// Making this global just keeps down on clutter
CPU cpu;

// Resets the CPU and loads the opcode into beginning of memory
void setup_test(OPCODES opcode) {
    cpu_reset(&cpu);
    cpu.memory[0] = opcode;
}

void test_MOV_A_A(void) {
    setup_test(MOV_A_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_B(void) {
    setup_test(MOV_A_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_C(void) {
    setup_test(MOV_A_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_D(void) {
    setup_test(MOV_A_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_E(void) {
    setup_test(MOV_A_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_H(void) {
    setup_test(MOV_A_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_L(void) {
    setup_test(MOV_A_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_A_M(void) {
    setup_test(MOV_A_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MOV_B_A(void) {
    setup_test(MOV_B_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_B(void) {
    setup_test(MOV_B_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_C(void) {
    setup_test(MOV_B_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_D(void) {
    setup_test(MOV_B_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_E(void) {
    setup_test(MOV_B_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_H(void) {
    setup_test(MOV_B_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_L(void) {
    setup_test(MOV_B_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_B_M(void) {
    setup_test(MOV_B_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MOV_C_A(void) {
    setup_test(MOV_C_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_B(void) {
    setup_test(MOV_C_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_C(void) {
    setup_test(MOV_C_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_D(void) {
    setup_test(MOV_C_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_E(void) {
    setup_test(MOV_C_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_H(void) {
    setup_test(MOV_C_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_L(void) {
    setup_test(MOV_C_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_C_M(void) {
    setup_test(MOV_C_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MOV_D_A(void) {
    setup_test(MOV_D_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_B(void) {
    setup_test(MOV_D_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_C(void) {
    setup_test(MOV_D_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_D(void) {
    setup_test(MOV_D_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_E(void) {
    setup_test(MOV_D_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_H(void) {
    setup_test(MOV_D_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_L(void) {
    setup_test(MOV_D_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_D_M(void) {
    setup_test(MOV_D_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MOV_E_A(void) {
    setup_test(MOV_E_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_B(void) {
    setup_test(MOV_E_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_C(void) {
    setup_test(MOV_E_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_D(void) {
    setup_test(MOV_E_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_E(void) {
    setup_test(MOV_E_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_H(void) {
    setup_test(MOV_E_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_L(void) {
    setup_test(MOV_E_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_E_M(void) {
    setup_test(MOV_E_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MOV_H_A(void) {
    setup_test(MOV_H_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_B(void) {
    setup_test(MOV_H_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_C(void) {
    setup_test(MOV_H_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_D(void) {
    setup_test(MOV_H_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_E(void) {
    setup_test(MOV_H_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_H(void) {
    setup_test(MOV_H_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_L(void) {
    setup_test(MOV_H_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_H_M(void) {
    setup_test(MOV_H_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MOV_L_A(void) {
    setup_test(MOV_L_A);
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_B(void) {
    setup_test(MOV_L_B);
    cpu.reg[B] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_C(void) {
    setup_test(MOV_L_C);
    cpu.reg[C] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_D(void) {
    setup_test(MOV_L_D);
    cpu.reg[D] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_E(void) {
    setup_test(MOV_L_E);
    cpu.reg[E] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_H(void) {
    setup_test(MOV_L_H);
    cpu.reg[H] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_L(void) {
    setup_test(MOV_L_L);
    cpu.reg[L] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_L_M(void) {
    setup_test(MOV_L_M);
    cpu.memory[0xDEAD] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MOV_M_A(void) {
    setup_test(MOV_M_A);
    cpu.reg[A] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_MOV_M_B(void) {
    setup_test(MOV_M_B);
    cpu.reg[B] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_MOV_M_C(void) {
    setup_test(MOV_M_C);
    cpu.reg[C] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_MOV_M_D(void) {
    setup_test(MOV_M_D);
    cpu.reg[D] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_MOV_M_E(void) {
    setup_test(MOV_M_E);
    cpu.reg[E] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_MOV_M_H(void) {
    setup_test(MOV_M_H);
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    // Strange opcode...
    assert(cpu.memory[0xDEAD] == 0xDE);
}

void test_MOV_M_L(void) {
    setup_test(MOV_M_L);
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    // Also strange opcode...
    assert(cpu.memory[0xDEAD] == 0xAD);
}

void test_MVI_A(void) {
    setup_test(MVI_A);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_MVI_B(void) {
    setup_test(MVI_B);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[B] == 0x42);
}

void test_MVI_C(void) {
    setup_test(MVI_C);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[C] == 0x42);
}

void test_MVI_D(void) {
    setup_test(MVI_D);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[D] == 0x42);
}

void test_MVI_E(void) {
    setup_test(MVI_E);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[E] == 0x42);
}

void test_MVI_H(void) {
    setup_test(MVI_H);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0x42);
}

void test_MVI_L(void) {
    setup_test(MVI_L);
    cpu.memory[1] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
}

void test_MVI_M(void) {
    setup_test(MVI_M);
    cpu.memory[1] = 0x42;
    cpu_set_reg_pair(&cpu, H, L, 0xDEAD);
    cpu_tick(&cpu);

    assert(cpu.memory[cpu_get_reg_pair(&cpu, H, L)] == 0x42);
}

void test_LXI_B(void) {
    setup_test(LXI_B);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu_tick(&cpu);

    assert(cpu_get_reg_pair(&cpu, B, C) == 0xDEAD);
}

void test_LXI_D(void) {
    setup_test(LXI_D);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu_tick(&cpu);

    assert(cpu_get_reg_pair(&cpu, D, E) == 0xDEAD);
}

void test_LXI_H(void) {
    setup_test(LXI_H);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu_tick(&cpu);

    assert(cpu_get_reg_pair(&cpu, H, L) == 0xDEAD);
}

void test_LXI_SP(void) {
    setup_test(LXI_SP);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu_tick(&cpu);

    assert(cpu.sp == 0xDEAD);
}

void test_LDA(void) {
    setup_test(LDA);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.memory[0xDEAD] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_STA(void) {
    setup_test(STA);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_LHLD(void) {
    setup_test(LHLD);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.memory[0xDEAD] = 0x42;
    cpu.memory[0xDEAE] = 0x69;
    cpu_tick(&cpu);

    assert(cpu.reg[L] == 0x42);
    assert(cpu.reg[H] == 0x69);
}

void test_SHLD(void) {
    setup_test(SHLD);
    cpu.memory[1] = 0xAD;
    cpu.memory[2] = 0xDE;
    cpu.reg[L] = 0x42;
    cpu.reg[H] = 0x69;
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
    assert(cpu.memory[0xDEAE] == 0x69);
}

void test_LDAX_B(void) {
    setup_test(LDAX_B);
    cpu.reg[B] = 0xDE;
    cpu.reg[C] = 0xAD;
    cpu.memory[0xDEAD] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_LDAX_D(void) {
    setup_test(LDAX_D);
    cpu.reg[D] = 0xDE;
    cpu.reg[E] = 0xAD;
    cpu.memory[0xDEAD] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.reg[A] == 0x42);
}

void test_STAX_B(void) {
    setup_test(STAX_B);
    cpu.reg[B] = 0xDE;
    cpu.reg[C] = 0xAD;
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_STAX_D(void) {
    setup_test(STAX_D);
    cpu.reg[D] = 0xDE;
    cpu.reg[E] = 0xAD;
    cpu.reg[A] = 0x42;
    cpu_tick(&cpu);

    assert(cpu.memory[0xDEAD] == 0x42);
}

void test_XCHG(void) {
    setup_test(XCHG);
    cpu.reg[H] = 0xDE;
    cpu.reg[L] = 0xAD;
    cpu.reg[D] = 0xBE;
    cpu.reg[E] = 0xEF;
    cpu_tick(&cpu);

    assert(cpu.reg[H] == 0xBE);
    assert(cpu.reg[L] == 0xEF);
    assert(cpu.reg[D] == 0xDE);
    assert(cpu.reg[E] == 0xAD);
}

int main(void) {
    cpu_init(&cpu);

    /* Data Transfer Tests */
    test_MOV_A_A();
    test_MOV_A_B();
    test_MOV_A_C();
    test_MOV_A_D();
    test_MOV_A_E();
    test_MOV_A_H();
    test_MOV_A_L();
    test_MOV_A_M();
    test_MOV_B_A();
    test_MOV_B_B();
    test_MOV_B_C();
    test_MOV_B_D();
    test_MOV_B_E();
    test_MOV_B_H();
    test_MOV_B_L();
    test_MOV_B_M();
    test_MOV_C_A();
    test_MOV_C_B();
    test_MOV_C_C();
    test_MOV_C_D();
    test_MOV_C_E();
    test_MOV_C_H();
    test_MOV_C_L();
    test_MOV_C_M();
    test_MOV_D_A();
    test_MOV_D_B();
    test_MOV_D_C();
    test_MOV_D_D();
    test_MOV_D_E();
    test_MOV_D_H();
    test_MOV_D_L();
    test_MOV_D_M();
    test_MOV_E_A();
    test_MOV_E_B();
    test_MOV_E_C();
    test_MOV_E_D();
    test_MOV_E_E();
    test_MOV_E_H();
    test_MOV_E_L();
    test_MOV_E_M();
    test_MOV_H_A();
    test_MOV_H_B();
    test_MOV_H_C();
    test_MOV_H_D();
    test_MOV_H_E();
    test_MOV_H_H();
    test_MOV_H_L();
    test_MOV_H_M();
    test_MOV_L_A();
    test_MOV_L_B();
    test_MOV_L_C();
    test_MOV_L_D();
    test_MOV_L_E();
    test_MOV_L_H();
    test_MOV_L_L();
    test_MOV_L_M();
    test_MOV_M_A();
    test_MOV_M_B();
    test_MOV_M_C();
    test_MOV_M_D();
    test_MOV_M_E();
    test_MOV_M_H();
    test_MOV_M_L();
    test_MVI_A();
    test_MVI_B();
    test_MVI_C();
    test_MVI_D();
    test_MVI_E();
    test_MVI_H();
    test_MVI_L();
    test_MVI_M();
    test_LXI_B();
    test_LXI_D();
    test_LXI_H();
    test_LXI_SP();
    test_LDA();
    test_STA();
    test_LHLD();
    test_SHLD();
    test_LDAX_B();
    test_LDAX_D();
    test_STAX_B();
    test_STAX_D();

    /* Arithmetic Tests */

    /* Logical Tests */

    /* Branch Tests */

    /* Stack/IO Tests */

    // If we got here, an assert never failed.
    printf("All tests passed!\n");
}
