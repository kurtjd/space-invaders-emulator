#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "cpu.h"

#define DISP_WIDTH 224
#define DISP_HEIGHT 256
#define DISP_SCALE 3
#define DISP_BYTES (DISP_HEIGHT * 28)

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

/* IO Ports */
uint8_t read_inp1(void);
uint8_t read_inp2(void);
void write_snd1(uint8_t data);
void write_snd2(uint8_t data);
uint8_t read_shift(void);
void write_shift(uint8_t data);
void write_shift_amnt(uint8_t data);
void write_watchdog(uint8_t data);

/* Display */
void display_draw(SDL_Window *window, SDL_Surface *surface, const CPU *cpu);

/* Audio */
bool audio_init(void);
void audio_quit(void);

/* Buttons */
bool handle_input(void);

#endif
