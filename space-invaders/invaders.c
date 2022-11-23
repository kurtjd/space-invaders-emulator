#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "cpu.h"

#define DISP_WIDTH 224
#define DISP_HEIGHT 256
#define DISP_SCALE 3
#define DISP_BYTES (DISP_HEIGHT * 28)

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

Mix_Chunk *invader_death_snd;
Mix_Chunk *invader_stp1_snd;
Mix_Chunk *invader_stp2_snd;
Mix_Chunk *invader_stp3_snd;
Mix_Chunk *invader_stp4_snd;
Mix_Chunk *player_death_snd;
Mix_Chunk *shoot_laser_snd;
Mix_Chunk *ufo_killed_snd;
Mix_Chunk *ufo_move_snd;

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
    return ((shift_reg >> (8 - shift_amnt)) & 0xFF);
}

void set_shift_amnt(uint8_t data) {
    shift_amnt = data & 7;
}

/* Input Port 0 */
uint8_t inp0_reg = 0;
uint8_t read_inp0(void) {
    return inp0_reg;
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
uint8_t snd1_reg = 0;
int ufo_move_chan = -1;
void write_sound1(uint8_t data) {
    if (data & (1 << 0)) {
        ufo_move_chan = Mix_PlayChannel(-1, ufo_move_snd, 0);
    }
    if (!(snd1_reg & (1 << 1)) && data & (1 << 1)) {
        Mix_PlayChannel(-1, shoot_laser_snd, 0);
    }
    if (!(snd1_reg & (1 << 2)) && data & (1 << 2)) {
        Mix_PlayChannel(-1, player_death_snd, 0);
    }
    if (!(snd1_reg & (1 << 3)) && data & (1 << 3)) {
        Mix_PlayChannel(-1, invader_death_snd, 0);
    }

    snd1_reg = data;
}

/* Sound Port 2 */
uint8_t snd2_reg = 0;
void write_sound2(uint8_t data) {
    if (!(snd2_reg & (1 << 1)) && data & (1 << 0)) {
        Mix_PlayChannel(-1, invader_stp1_snd, 0);
    }
    if (!(snd2_reg & (1 << 1)) && data & (1 << 1)) {
        Mix_PlayChannel(-1, invader_stp2_snd, 0);
    }
    if (!(snd2_reg & (1 << 2)) && data & (1 << 2)) {
        Mix_PlayChannel(-1, invader_stp3_snd, 0);
    }
    if (!(snd2_reg & (1 << 3)) && data & (1 << 3)) {
        Mix_PlayChannel(-1, invader_stp4_snd, 0);
    }
    if (!(snd2_reg & (1 << 4)) && data & (1 << 4)) {
        Mix_HaltChannel(ufo_move_chan); // Otherwise kill sound wont play
        Mix_PlayChannel(-1, ufo_killed_snd, 0);
    }

    snd2_reg = data;
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Create the SDL window.
SDL_Window *create_window()
{
    int window_width = DISP_WIDTH * DISP_SCALE;
    int window_height = DISP_HEIGHT * DISP_SCALE;

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
            } else if (keyc == SDLK_t) {
                set_port_bit(&inp2_reg, 2, true);
            } else if (keyc == SDLK_SPACE) {
                set_port_bit(&inp1_reg, 4, true);
                set_port_bit(&inp2_reg, 4, true);
            } else if (keyc == SDLK_RIGHT) {
                set_port_bit(&inp1_reg, 6, true);
                set_port_bit(&inp2_reg, 6, true);
            } else if (keyc == SDLK_LEFT) {
                set_port_bit(&inp1_reg, 5, true);
                set_port_bit(&inp2_reg, 5, true);
            } else if (keyc == SDLK_2) {
                set_port_bit(&inp1_reg, 1, true);
            }
            break;
        case SDL_KEYUP:
            if (keyc == SDLK_RETURN) {
                set_port_bit(&inp1_reg, 2, false);
            } else if (keyc == SDLK_c) {
                set_port_bit(&inp1_reg, 0, false);
            } else if (keyc == SDLK_t) {
                set_port_bit(&inp2_reg, 2, false);
            } else if (keyc == SDLK_SPACE) {
                set_port_bit(&inp1_reg, 4, false);
                set_port_bit(&inp2_reg, 4, false);
            } else if (keyc == SDLK_RIGHT) {
                set_port_bit(&inp1_reg, 6, false);
                set_port_bit(&inp2_reg, 6, false);
            } else if (keyc == SDLK_LEFT) {
                set_port_bit(&inp1_reg, 5, false);
                set_port_bit(&inp2_reg, 5, false);
            } else if (keyc == SDLK_2) {
                set_port_bit(&inp1_reg, 1, false);
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

// This is yucky. Will clean up later.
void draw_display(SDL_Window *window, SDL_Surface *surface, const CPU *cpu) {
    for (int i = 0; i < DISP_BYTES; i++) {
        uint8_t byte = cpu->memory[VIDEO_MEMORY_START + i];

        int y = (((DISP_HEIGHT * DISP_SCALE) - 1) - ((i % 32) * (8 * (DISP_SCALE))));
        int x = ((i / 32)) * DISP_SCALE;

        for (int k = 0; k < 8; k++) {
            int tmp_y = y - (k * (DISP_SCALE));

            for (int ys = 0; ys < DISP_SCALE; ys++) {
                for (int xs = 0; xs < DISP_SCALE; xs++) {
                    int final_x = x + xs;
                    int final_y = tmp_y - ys;
                    long color;

                    if (final_y >= 0 && final_y < 32 * DISP_SCALE) {
                        color = 0xFFFFFF;
                    }else if (final_y >= 32 * DISP_SCALE && final_y < 64 * DISP_SCALE) {
                        color = 0xFF0000;
                    } else if (final_y >= 64 * DISP_SCALE && final_y < 184 * DISP_SCALE) {
                        color = 0xFFFFFF;
                    } else if (final_y >= 184 * DISP_SCALE && final_y < 240 * DISP_SCALE) {
                        color = 0x00FF00;
                    } else if (final_y >= 240 * DISP_SCALE && final_y < 256 * DISP_SCALE && final_x >= 0 && final_x < 16 * DISP_SCALE) {
                        color = 0xFFFFFF;
                    } else if (final_y >= 240 * DISP_SCALE && final_y < 256 * DISP_SCALE && final_x >= 16 * DISP_SCALE && final_x < 134 * DISP_SCALE) {
                        color = 0x00FF00;
                    } else if (final_y >= 240 * DISP_SCALE && final_y < 256 * DISP_SCALE && final_x >= 134 * DISP_SCALE && final_x < 224 * DISP_SCALE) {
                        color = 0xFFFFFF;
                    }

                    if (byte & (1 << k)) {
                        set_pixel(surface, final_x, final_y, color);
                    } else {
                        set_pixel(surface, final_x, final_y, 0x000000);
                    }
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}

void init_audio(void) {
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    invader_death_snd = Mix_LoadWAV("../sound/sounds_invader_death.wav");
    invader_stp1_snd = Mix_LoadWAV("../sound/sounds_invader_step1.wav");
    invader_stp2_snd = Mix_LoadWAV("../sound/sounds_invader_step2.wav");
    invader_stp3_snd = Mix_LoadWAV("../sound/sounds_invader_step3.wav");
    invader_stp4_snd = Mix_LoadWAV("../sound/sounds_invader_step4.wav");
    player_death_snd = Mix_LoadWAV("../sound/sounds_player_death.wav");
    shoot_laser_snd = Mix_LoadWAV("../sound/sounds_shoot_laser.wav");
    ufo_killed_snd = Mix_LoadWAV("../sound/sounds_ufo_killed.wav");
    ufo_move_snd = Mix_LoadWAV("../sound/sounds_ufo_move.wav");
}

int main(void) {
    CPU cpu;
    cpu_init(&cpu);
    cpu_load_rom(&cpu, "../roms/space-invaders/invaders.bin", 0x0000);

    port_init(&cpu);

    init_SDL();
    init_audio();
    SDL_Window *window = create_window();
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Event e;

    //int timer = 0;
    while(!cpu.exit) {
        /*if (timer >= 1000) {
            if ((cpu.total_cycles % VBLANK_RATE == 0)) {
                cpu_req_interrupt(&cpu, 2);
                draw_display(window, surface, &cpu);
                cpu.exit = !handle_input(&e);
            } else if ((cpu.total_cycles % ((VBLANK_RATE / 2) + 1) == 0)) {
                cpu_req_interrupt(&cpu, 1);
            }

            cpu_tick(&cpu);

            timer = 0;
        }

        timer++;*/

        uint32_t ticks = SDL_GetTicks();
        while (cpu.total_cycles % VBLANK_RATE != 0) {
            if ((cpu.total_cycles % ((VBLANK_RATE / 2) + 1) == 0)) {
                cpu_req_interrupt(&cpu, 1);
            }
            cpu_tick(&cpu);
        }
        cpu_req_interrupt(&cpu, 2);

        do {
            cpu_tick(&cpu);
        } while (!cpu.instr_complete);

        SDL_Delay((1000 / REFRESH_RATE) - (SDL_GetTicks() - ticks));
        draw_display(window, surface, &cpu);
        cpu.exit = !handle_input(&e);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    Mix_Quit();
    SDL_Quit();
}
