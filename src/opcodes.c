#include "opcodes.h"

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

/* Sets zero flag equal to 1 if res == 0, otherwise set to 0. */
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

/* Called by add-related opcodes that follow standard flag update behavior. */
static void _update_flags_add(CPU *cpu, uint8_t val1, uint8_t val2, bool carry) {
    uint8_t res = val1 + val2;

    _update_flag_z(cpu, res);
    _update_flag_p(cpu, res);
    _update_flag_s(cpu, res);
    _update_flag_ac_add(cpu, val1, val2, carry);
    _update_flag_cy_add(cpu, val1, val2, carry);
}

/* Called by subract-related opcodes that follow standard flag update behavior. */
static void _update_flags_sub(CPU *cpu, uint8_t val1, uint8_t val2, bool carry) {
    uint8_t res = val1 - val2;

    _update_flag_z(cpu, res);
    _update_flag_p(cpu, res);
    _update_flag_s(cpu, res);
    _update_flag_ac_sub(cpu, val1, val2, carry);
    _update_flag_cy_sub(cpu, val1, val2, carry);
}

/* Called by inc-related opcodes that follow standard flag update behavior. */
static void _update_flags_inc(CPU *cpu, uint8_t val) {
    uint8_t res = val + 1;

    _update_flag_z(cpu, res);
    _update_flag_p(cpu, res);
    _update_flag_s(cpu, res);
    _update_flag_ac_add(cpu, val, 1, false);
}

/* Called by dec-related opcodes that follow standard flag update behavior. */
static void _update_flags_dec(CPU *cpu, uint8_t val) {
    uint8_t res = val - 1;

    _update_flag_z(cpu, res);
    _update_flag_p(cpu, res);
    _update_flag_s(cpu, res);
    _update_flag_ac_sub(cpu, val, 1, false);
}

/* Called by logical-related opcodes that follow standard flag update behavior. */
static void _update_flags_log(CPU *cpu, uint8_t val) {
    _update_flag_z(cpu, val);
    _update_flag_p(cpu, val);
    _update_flag_s(cpu, val);
}

/* Called by and-related opcodes that follow standard flag update behavior. */
static void _update_flags_and(CPU *cpu, uint8_t val1, uint8_t val2) {
    _update_flags_log(cpu, val1 & val2);
    cpu_set_flag_bit(cpu, CY, false);

    // Weird note: Logical AND ops set AC to the logical or of bit 3
    cpu_set_flag_bit(cpu, AC, ((val1 | val2) >> 3) & 1);
}

/* Called by or-related opcodes that follow standard flag update behavior. */
static void _update_flags_or(CPU *cpu, uint8_t val) {
    _update_flags_log(cpu, val);
    _clear_cy_ac(cpu);
}

/* Simply swaps two values */
static void _swap(uint8_t *a, uint8_t *b) {
    uint8_t tmp = *a;
    *a = *b;
    *b = tmp;
}


void MOV_R_R(CPU *cpu, REGISTERS dest, REGISTERS src) {
    cpu->reg[dest] = cpu->reg[src];
}

void MOV_R_M(CPU *cpu, REGISTERS dest) {
    cpu->reg[dest] = cpu->memory[cpu_get_reg_pair(cpu, H, L)];
}

void MOV_M_R(CPU *cpu, REGISTERS src) {
    cpu->memory[cpu_get_reg_pair(cpu, H, L)] = cpu->reg[src];
}

void MVI_R(CPU *cpu, REGISTERS dest, uint8_t operand) {
    cpu->reg[dest] = operand;
}

void MVI_M(CPU *cpu, uint8_t operand) {
    cpu->memory[cpu_get_reg_pair(cpu, H, L)] = operand;
}

void LXI_RP(CPU *cpu, REGISTERS dest, const uint8_t operands[MAX_OPERANDS]) {
    cpu_set_reg_pair(cpu, dest, dest + 1, (operands[1] << 8) | operands[0]);
}

void LXI_SP(CPU *cpu, const uint8_t operands[MAX_OPERANDS]) {
    cpu->sp = (operands[1] << 8) | operands[0];
}

void LDA(CPU *cpu, const uint8_t operands[MAX_OPERANDS]) {
    cpu->reg[A] = cpu->memory[(operands[1] << 8) | operands[0]];
}

void STA(CPU *cpu, const uint8_t operands[MAX_OPERANDS]) {
    cpu->memory[(operands[1] << 8) | operands[0]] = cpu->reg[A];
}

void LHLD(CPU *cpu, const uint8_t operands[MAX_OPERANDS]) {
    uint16_t addr = (operands[1] << 8) | operands[0];
    cpu->reg[L] = cpu->memory[addr];
    cpu->reg[H] = cpu->memory[addr + 1];
}

void SHLD(CPU *cpu, const uint8_t operands[MAX_OPERANDS]) {
    uint16_t addr = (operands[1] << 8) | operands[0];
    cpu->memory[addr] = cpu->reg[L];
    cpu->memory[addr + 1] = cpu->reg[H];
}

void LDAX_RP(CPU *cpu, REGISTERS src) {
    cpu->reg[A] = cpu->memory[cpu_get_reg_pair(cpu, src, src + 1)];
}

void STAX_RP(CPU *cpu, REGISTERS src) {
    cpu->memory[cpu_get_reg_pair(cpu, src, src + 1)] = cpu->reg[A];
}

void XCHG(CPU *cpu) {
    _swap(&cpu->reg[H], &cpu->reg[D]);
    _swap(&cpu->reg[L], &cpu->reg[E]);
}

void ADD_R(CPU *cpu, REGISTERS src) {
    _update_flags_add(cpu, cpu->reg[A], cpu->reg[src], false);
    cpu->reg[A] += cpu->reg[src];
}

void ADD_M(CPU *cpu) {
    uint8_t val = cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_add(cpu, cpu->reg[A], val, false);
    cpu->reg[A] += val;
}

void ADI(CPU *cpu, uint8_t operand) {
    _update_flags_add(cpu, cpu->reg[A], operand, false);
    cpu->reg[A] += operand;
}

void ADC_R(CPU *cpu, REGISTERS src) {
    _update_flags_add(cpu, cpu->reg[A], cpu->reg[src], true);
    cpu->reg[A] += (cpu->reg[src] + cpu_get_flag_bit(cpu, CY));
}

void ADC_M(CPU *cpu) {
    uint8_t val = cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_add(cpu, cpu->reg[A], val, true);
    cpu->reg[A] += (val + cpu_get_flag_bit(cpu, CY));
}

void ACI(CPU *cpu, uint8_t operand) {
    _update_flags_add(cpu, cpu->reg[A], operand, true);
    cpu->reg[A] += (operand + cpu_get_flag_bit(cpu, CY));
}

void SUB_R(CPU *cpu, REGISTERS src) {
    _update_flags_sub(cpu, cpu->reg[A], cpu->reg[src], false);
    cpu->reg[A] -= cpu->reg[src];
}

void SUB_M(CPU *cpu) {
    uint8_t val = cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_sub(cpu, cpu->reg[A], val, false);
    cpu->reg[A] -= val;
}

void SUI(CPU *cpu, uint8_t operand) {
    _update_flags_sub(cpu, cpu->reg[A], operand, false);
    cpu->reg[A] -= operand;
}

void SBB_R(CPU *cpu, REGISTERS src) {
    _update_flags_sub(cpu, cpu->reg[A], cpu->reg[src], true);
    cpu->reg[A] -= (cpu->reg[src] + cpu_get_flag_bit(cpu, CY));
}

void SBB_M(CPU *cpu) {
    uint8_t val = cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_sub(cpu, cpu->reg[A], val, true);
    cpu->reg[A] -= (val + cpu_get_flag_bit(cpu, CY));
}

void SBI(CPU *cpu, uint8_t operand) {
    _update_flags_sub(cpu, cpu->reg[A], operand, true);
    cpu->reg[A] -= (operand + cpu_get_flag_bit(cpu, CY));
}

void INR_R(CPU *cpu, REGISTERS dest) {
    _update_flags_inc(cpu, cpu->reg[dest]);
    cpu->reg[dest]++;
}

void INR_M(CPU *cpu) {
    _update_flags_inc(cpu, cpu->memory[cpu_get_reg_pair(cpu, H, L)]);
    cpu->memory[cpu_get_reg_pair(cpu, H, L)]++;
}

void DCR_R(CPU *cpu, REGISTERS dest) {
    _update_flags_dec(cpu, cpu->reg[dest]);
    cpu->reg[dest]--;
}

void DCR_M(CPU *cpu) {
    _update_flags_dec(cpu, cpu->memory[cpu_get_reg_pair(cpu, H, L)]);
    cpu->memory[cpu_get_reg_pair(cpu, H, L)]--;
}

void INX_RP(CPU *cpu, REGISTERS dest) {
    uint16_t val = cpu_get_reg_pair(cpu, dest, dest + 1) + 1;
    cpu_set_reg_pair(cpu, dest, dest + 1, val);
}

void INX_SP(CPU *cpu) {
    cpu->sp++;
}

void DCX_RP(CPU *cpu, REGISTERS dest) {
    uint16_t val = cpu_get_reg_pair(cpu, dest, dest + 1) - 1;
    cpu_set_reg_pair(cpu, dest, dest + 1, val);
}

void DCX_SP(CPU *cpu) {
    cpu->sp--;
}

void DAD_RP(CPU *cpu, REGISTERS src) {
    uint32_t res = cpu_get_reg_pair(cpu, H, L) + cpu_get_reg_pair(cpu, src, src + 1);
    cpu_set_flag_bit(cpu, CY, res > 0xFFFF);
    cpu_set_reg_pair(cpu, H, L, res);
}

void DAD_SP(CPU *cpu) {
    uint32_t res = cpu_get_reg_pair(cpu, H, L) + cpu->sp;
    cpu_set_flag_bit(cpu, CY, res > 0xFFFF);
    cpu_set_reg_pair(cpu, H, L, res);
}

void DAA(CPU *cpu) {
    // Complicated. Come back to this one.

    if (cpu_get_flag_bit(cpu, AC) || ((cpu->reg[A] & 0xF) > 9)) {
        cpu->reg[A] += 6;
    }

    if (cpu_get_flag_bit(cpu, CY) || ((cpu->reg[A] >> 4) > 9)) {
        uint8_t lsb = cpu->reg[A] & 0xF;
        uint8_t msb = (cpu->reg[A] >> 4) + 6;
        cpu->reg[A] = (msb << 4) | lsb;
    }
}

void ANA_R(CPU *cpu, REGISTERS src) {
    _update_flags_and(cpu, cpu->reg[A], cpu->reg[src]);
    cpu->reg[A] &= cpu->reg[src];
}

void ANA_M(CPU *cpu) {
    uint8_t val = cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_and(cpu, cpu->reg[A], val);
    cpu->reg[A] &= val;
}

void ANI(CPU *cpu, uint8_t operand) {
    _update_flags_and(cpu, cpu->reg[A], operand);
    cpu_set_flag_bit(cpu, AC, false); // This particular AND clears AC
    cpu->reg[A] &= operand;
}

void XRA_R(CPU *cpu, REGISTERS src) {
    uint8_t res = cpu->reg[A] ^ cpu->reg[src];
    _update_flags_or(cpu, res);
    cpu->reg[A] = res;
}

void XRA_M(CPU *cpu) {
    uint8_t res = cpu->reg[A] ^ cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_or(cpu, res);
    cpu->reg[A] = res;
}

void XRI(CPU *cpu, uint8_t operand) {
    uint8_t res = cpu->reg[A] ^ operand;
    _update_flags_or(cpu, res);
    cpu->reg[A] = res;
}

void ORA_R(CPU *cpu, REGISTERS src) {
    uint8_t res = cpu->reg[A] | cpu->reg[src];
    _update_flags_or(cpu, res);
    cpu->reg[A] = res;
}

void ORA_M(CPU *cpu) {
    uint8_t res = cpu->reg[A] | cpu->memory[cpu_get_reg_pair(cpu, H, L)];
    _update_flags_or(cpu, res);
    cpu->reg[A] = res;
}

void ORI(CPU *cpu, uint8_t operand) {
    uint8_t res = cpu->reg[A] | operand;
    _update_flags_or(cpu, res);
    cpu->reg[A] = res;
}
