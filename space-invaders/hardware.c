#include <SDL2/SDL_mixer.h>
#include "hardware.h"

/* Registers for the IO ports to work with */
static uint8_t inp1_reg = 0;
static uint8_t inp2_reg = 0;
static uint8_t snd1_reg = 0;
static uint8_t snd2_reg = 0;
static uint16_t shift_reg = 0;
static uint8_t shift_amnt_reg = 0;

typedef enum {
    CREDIT = 1,
    START_2P = (1 << 1),
    START_1P = (1 << 2),
    SHOT_1P = (1 << 4),
    LEFT_1P = (1 << 5),
    RIGHT_1P = (1 << 6),

    TILT = (1 << 2),
    SHOT_2P = (1 << 4),
    LEFT_2P = (1 << 5),
    RIGHT_2P = (1 << 6),

    UFO = 1,
    SHOT = (1 << 1),
    PLAYER_DIE = (1 << 2),
    INVADER_DIE = (1 << 3),
    FLEET_MOVE1 = 1,
    FLEET_MOVE2 = (1 << 1),
    FLEET_MOVE3 = (1 << 2),
    FLEET_MOVE4 = (1 << 3),
    UFO_HIT = (1 << 4)
} PORT_BITS;

/* Sound instances */
static Mix_Chunk *invader_death_snd;
static Mix_Chunk *invader_stp1_snd;
static Mix_Chunk *invader_stp2_snd;
static Mix_Chunk *invader_stp3_snd;
static Mix_Chunk *invader_stp4_snd;
static Mix_Chunk *player_death_snd;
static Mix_Chunk *shoot_laser_snd;
static Mix_Chunk *ufo_killed_snd;
static Mix_Chunk *ufo_move_snd;

// Needed for lame audio bug
static int ufo_move_chan = -1;

// Sets a pixel of the SDL surface to a certain color.
static void _set_pixel(SDL_Surface *surface, int x, int y, long color)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = color;
}

uint8_t read_inp1(void) {
    return inp1_reg;
}

uint8_t read_inp2(void) {
    return inp2_reg;
}

void write_snd1(uint8_t data) {
    if (data & UFO) { // This sound keeps playing
        ufo_move_chan = Mix_PlayChannel(-1, ufo_move_snd, 0);
    }
    if (!(snd1_reg & SHOT) && (data & SHOT)) {
        Mix_PlayChannel(-1, shoot_laser_snd, 0);
    }
    if (!(snd1_reg & PLAYER_DIE) && (data & PLAYER_DIE)) {
        Mix_PlayChannel(-1, player_death_snd, 0);
    }
    if (!(snd1_reg & INVADER_DIE) && (data & INVADER_DIE)) {
        Mix_PlayChannel(-1, invader_death_snd, 0);
    }

    snd1_reg = data;
}

void write_snd2(uint8_t data) {
    if (!(snd2_reg & FLEET_MOVE1) && (data & FLEET_MOVE1)) {
        Mix_PlayChannel(-1, invader_stp1_snd, 0);
    }
    if (!(snd2_reg & FLEET_MOVE2) && (data & FLEET_MOVE2)) {
        Mix_PlayChannel(-1, invader_stp2_snd, 0);
    }
    if (!(snd2_reg & FLEET_MOVE3) && (data & FLEET_MOVE3)) {
        Mix_PlayChannel(-1, invader_stp3_snd, 0);
    }
    if (!(snd2_reg & FLEET_MOVE4) && (data & FLEET_MOVE4)) {
        Mix_PlayChannel(-1, invader_stp4_snd, 0);
    }
    if (!(snd2_reg & UFO_HIT) && (data & UFO_HIT)) {
        Mix_HaltChannel(ufo_move_chan); // Otherwise kill sound wont play
        Mix_PlayChannel(-1, ufo_killed_snd, 0);
    }

    snd2_reg = data;
}

uint8_t read_shift(void) {
    return (shift_reg >> (8 - shift_amnt_reg));
}

void write_shift(uint8_t data) {
    shift_reg >>= 8;
    shift_reg |= (data << 8);
}

void write_shift_amnt(uint8_t data) {
    shift_amnt_reg = data & 7;
}

void write_watchdog(uint8_t data) {
    // No functionality needed
}

// This is yucky. Will clean up later.
void display_draw(SDL_Window *window, SDL_Surface *surface, const CPU *cpu) {
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
                        _set_pixel(surface, final_x, final_y, color);
                    } else {
                        _set_pixel(surface, final_x, final_y, 0x000000);
                    }
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
}

void audio_init(void) {
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    invader_death_snd = Mix_LoadWAV("../sound/invader_death.wav");
    invader_stp1_snd = Mix_LoadWAV("../sound/fleet1.wav");
    invader_stp2_snd = Mix_LoadWAV("../sound/fleet2.wav");
    invader_stp3_snd = Mix_LoadWAV("../sound/fleet3.wav");
    invader_stp4_snd = Mix_LoadWAV("../sound/fleet4.wav");
    player_death_snd = Mix_LoadWAV("../sound/player_death.wav");
    shoot_laser_snd = Mix_LoadWAV("../sound/shoot_laser.wav");
    ufo_killed_snd = Mix_LoadWAV("../sound/ufo_hit.wav");
    ufo_move_snd = Mix_LoadWAV("../sound/ufo_move.wav");
}

void audio_quit(void) {
    Mix_Quit();
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
                inp1_reg |= START_1P;
            } else if (keyc == SDLK_c) {
                inp1_reg |= CREDIT;
            } else if (keyc == SDLK_t) {
                inp2_reg |= TILT;
            } else if (keyc == SDLK_SPACE) {
                inp1_reg |= SHOT_1P;
                inp2_reg |= SHOT_2P;
            } else if (keyc == SDLK_RIGHT) {
                inp1_reg |= RIGHT_1P;
                inp2_reg |= RIGHT_2P;
            } else if (keyc == SDLK_LEFT) {
                inp1_reg |= LEFT_1P;
                inp2_reg |= LEFT_2P;
            } else if (keyc == SDLK_2) {
                inp1_reg |= START_2P;
            }
            break;
        case SDL_KEYUP:
            if (keyc == SDLK_RETURN) {
                inp1_reg &= ~START_1P;
            } else if (keyc == SDLK_c) {
                inp1_reg &= ~CREDIT;
            } else if (keyc == SDLK_t) {
                inp2_reg &= ~TILT;
            } else if (keyc == SDLK_SPACE) {
                inp1_reg &= ~SHOT_1P;
                inp2_reg &= ~SHOT_2P;
            } else if (keyc == SDLK_RIGHT) {
                inp1_reg &= ~RIGHT_1P;
                inp2_reg &= ~RIGHT_2P;
            } else if (keyc == SDLK_LEFT) {
                inp1_reg &= ~LEFT_1P;
                inp2_reg &= ~LEFT_2P;
            } else if (keyc == SDLK_2) {
                inp1_reg &= ~START_2P;
            }
            break;
        }
    }

    return true;
}
