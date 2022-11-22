#include <stdio.h>
#include "cpu.h"

#define REFRESH_RATE 60
#define CPU_CLOCK 2000000
#define VBLANK_RATE (CPU_CLOCK / REFRESH_RATE)
#define VIDEO_MEMORY_START 0x2400

#define INP1 1
#define INP2 2

#define SHFT_IN 3
#define SHFTAMNT 2
#define SHFT_DATA 4

#define SOUND1 3
#define SOUND2 5

#define WATCHDOG 6

typedef enum {
    CREDIT = 1,
    START_2P = (1 << 1),
    START_1P = (1 << 2),
    SHOT_1P = (1 << 4),
    LEFT_1P = (1 << 5),
    RIGHT_1P = (1 << 6),
} INP1_BITS;

void set_port_bit(uint8_t *port, uint8_t bit, bool high) {
    if (high) {
        *port |= bit;
    } else {
        *port &= ~bit;
    }
}

/* Shift Port */
uint16_t shift_reg = 0;
uint8_t shift_amnt = 0;
void write_shift(uint8_t data) {
    shift_reg >>= 8;
    shift_reg |= (data << 8);
}

uint8_t read_shift(void) {
    return (shift_reg >> (8 - shift_amnt));
}

void set_shift_amnt(uint8_t data) {
    shift_amnt = data;
}

/* Input Port 1 */
uint8_t inp1_reg = 0;
uint8_t read_inp1(void) {
    return inp1_reg;
}

/* Input Port 2 */
uint8_t inp2_reg = 0;
uint8_t read_inp2(void) {
    return inp2_reg;
}

/* Sound Port 1 */
void write_sound1(uint8_t data) {
    (void)data;
}

/* Sound Port 2 */
void write_sound2(uint8_t data) {
    (void)data;
}

/* Watchdog */
void write_watchdog(uint8_t data) {
    (void)data;
}

void port_init(CPU *cpu) {
    cpu->input[INP1] = &read_inp1;
    cpu->input[INP2] = &read_inp2;

    cpu->input[SHFT_IN] = &read_shift;
    cpu->output[SHFT_DATA] = &write_shift;
    cpu->output[SHFTAMNT] = &set_shift_amnt;

    cpu->output[SOUND1] = &write_sound1;
    cpu->output[SOUND2] = &write_sound2;

    cpu->output[WATCHDOG] = &write_watchdog;
}

int main(void) {
    CPU cpu;
    cpu_init(&cpu);
    cpu_load_rom(&cpu, "../roms/space-invaders/invaders.bin", 0x0000);

    port_init(&cpu);

    int instr_count = 0;
    while(!cpu.exit) {
        if (cpu.instr_complete) {
            instr_count++;
        }
        if (instr_count >= 1000 && cpu.instr_complete) {
            cpu_print_debug(&cpu);
            getchar();
        }

        if ((cpu.total_cycles % VBLANK_RATE == 0)) {
            cpu_interrupt(&cpu, 2);
        } else if ((cpu.total_cycles % (VBLANK_RATE / 2) == 0)) {
            cpu_interrupt(&cpu, 1);
        }

        cpu_tick(&cpu);
    }
}
