#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu.h"

#define REFRESH_RATE 60
#define CPU_CLOCK 2000000
#define VBLANK_RATE (CPU_CLOCK / REFRESH_RATE)
#define VIDEO_MEMORY_START 0x2400

#define INP0 0
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
        *port |= (1 << bit);
    } else {
        *port &= ~(1 << bit);
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
uint8_t inp0_reg = 0xFF;
uint8_t read_inp0(void) {
    return inp0_reg;
}

/* Input Port 1 */
uint8_t inp1_reg = (1 << 3);
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
    cpu->input[INP0] = read_inp0;
    cpu->input[INP1] = read_inp1;
    cpu->input[INP2] = read_inp2;

    cpu->input[SHFT_IN] = &read_shift;
    cpu->output[SHFT_DATA] = &write_shift;
    cpu->output[SHFTAMNT] = &set_shift_amnt;

    cpu->output[SOUND1] = &write_sound1;
    cpu->output[SOUND2] = &write_sound2;

    cpu->output[WATCHDOG] = &write_watchdog;

    cpu->input[INP1]();
}

// Initializes SDL.
bool init_SDL()
{
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Create the SDL window.
SDL_Window *create_window()
{
    int window_width = 224;
    int window_height = 256;

    SDL_Window *new_window = SDL_CreateWindow("Space Invaders",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              window_width,
                                              window_height,
                                              SDL_WINDOW_SHOWN);
    if (!new_window)
    {
        fprintf(stderr, "Could not create SDL window: %s\n", SDL_GetError());
        return NULL;
    }

    return new_window;
}

bool handle_input(SDL_Event *e) {
    while (SDL_PollEvent(e)) {
        SDL_Keycode keyc = e->key.keysym.sym;

        switch (e->type) {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            if (keyc == SDLK_RETURN) {
                set_port_bit(&inp1_reg, 2, true);
            } else if (keyc == SDLK_c) {
                set_port_bit(&inp1_reg, 0, true);
            }
            break;
        case SDL_KEYUP:
            if (keyc == SDLK_RETURN) {
                set_port_bit(&inp1_reg, 2, false);
            }
            break;
        }
    }

    return true;
}

// Sets a pixel of the SDL surface to a certain color.
void set_pixel(SDL_Surface *surface, int x, int y, long color)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = color;
}

void draw_display(SDL_Window *window, SDL_Surface *surface, const CPU *cpu) {
    for (int i = 0; i < 256 * 28; i++) {
        uint8_t byte = cpu->memory[VIDEO_MEMORY_START + i];
        int y = 255 - ((i % 32) * 8);
        int x = i / 32;

        for (int k = 0; k < 7; k++) {
            int tmp_y = y - k;

            if (byte & (1 << k)) {
                set_pixel(surface, x, tmp_y, 0xFFFFFFFF);
            } else {
                set_pixel(surface, x, tmp_y, 0x00000000);
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}

int main(void) {
    CPU cpu;
    cpu_init(&cpu);
    cpu_load_rom(&cpu, "../roms/space-invaders/invaders.bin", 0x0000);

    port_init(&cpu);

    init_SDL();
    SDL_Window *window = create_window();
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Event e;

    int timer = 0;
    while(!cpu.exit) {
        if (timer >= 1000) {
            //printf("HAX\n");

            if ((cpu.total_cycles % VBLANK_RATE == 0)) {
                cpu_interrupt(&cpu, 2);
                draw_display(window, surface, &cpu);
                cpu.exit = !handle_input(&e);
            } else if ((cpu.total_cycles % (VBLANK_RATE / 2) == 0)) {
                cpu_interrupt(&cpu, 1);
            }

            cpu_tick(&cpu);

            timer = 0;
        }

        timer++;
    }

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
