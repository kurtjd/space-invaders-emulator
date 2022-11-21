#include <stdio.h>
#include "cpu.h"

#define REFRESH_RATE 60
#define CPU_CLOCK 2000000
#define VBLANK_RATE (CPU_CLOCK / REFRESH_RATE)

int main(void) {
    CPU cpu;
    cpu_init(&cpu);
    cpu_load_rom(&cpu, "../roms/space-invaders/invaders.bin", 0x0000);

    while(!cpu.exit) {
        if (cpu.total_cycles >= 50000000 && cpu.instr_complete) {
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
