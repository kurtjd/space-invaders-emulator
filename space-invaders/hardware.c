#include <SDL2/SDL_mixer.h>
#include "hardware.h"

#define NUM_SOUNDS 9

static SDL_Event e;

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

/* Registers for the IO ports to work with */
static uint8_t inp1_reg = 0;
static uint8_t inp2_reg = 0;
static uint8_t snd1_reg = 0;
static uint8_t snd2_reg = 0;
static uint16_t shift_reg = 0;
static uint8_t shift_amnt_reg = 0;

typedef enum {
    INVADER_DEATH_SND,
    PLAYER_DEATH_SND,
    SHOOT_LASER_SND,
    UFO_HIT_SND,
    UFO_MOVE_SND,
    FLEET1_SND,
    FLEET2_SND,
    FLEET3_SND,
    FLEET4_SND
} SOUND_INST;

/* Sound instances */
static Mix_Chunk *sounds[NUM_SOUNDS];

// Needed for lame audio bug
static int ufo_move_chan = -1;

// Sets a pixel of the SDL surface to a certain color.
static void _set_pixel(SDL_Surface *surface, int x, int y, long color) {
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
    if (data & UFO && sounds[UFO_MOVE_SND]) { // This sound keeps playing
        ufo_move_chan = Mix_PlayChannel(-1, sounds[UFO_MOVE_SND], 0);
    }
    if (!(snd1_reg & SHOT) && (data & SHOT) && sounds[SHOOT_LASER_SND]) {
        Mix_PlayChannel(-1, sounds[SHOOT_LASER_SND], 0);
    }
    if (!(snd1_reg & PLAYER_DIE) && (data & PLAYER_DIE) && sounds[PLAYER_DEATH_SND]) {
        Mix_PlayChannel(-1, sounds[PLAYER_DEATH_SND], 0);
    }
    if (!(snd1_reg & INVADER_DIE) && (data & INVADER_DIE) && sounds[INVADER_DEATH_SND]) {
        Mix_PlayChannel(-1, sounds[INVADER_DEATH_SND], 0);
    }

    snd1_reg = data;
}

void write_snd2(uint8_t data) {
    if (!(snd2_reg & FLEET_MOVE1) && (data & FLEET_MOVE1) && sounds[FLEET1_SND]) {
        Mix_PlayChannel(-1, sounds[FLEET1_SND], 0);
    }
    if (!(snd2_reg & FLEET_MOVE2) && (data & FLEET_MOVE2) && sounds[FLEET1_SND]) {
        Mix_PlayChannel(-1, sounds[FLEET2_SND], 0);
    }
    if (!(snd2_reg & FLEET_MOVE3) && (data & FLEET_MOVE3) && sounds[FLEET3_SND]) {
        Mix_PlayChannel(-1, sounds[FLEET3_SND], 0);
    }
    if (!(snd2_reg & FLEET_MOVE4) && (data & FLEET_MOVE4) && sounds[FLEET4_SND]) {
        Mix_PlayChannel(-1, sounds[FLEET4_SND], 0);
    }
    if (!(snd2_reg & UFO_HIT) && (data & UFO_HIT) && sounds[UFO_HIT_SND]) {
        Mix_HaltChannel(ufo_move_chan); // Otherwise kill sound wont play
        Mix_PlayChannel(-1, sounds[UFO_HIT_SND], 0);
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
    shift_amnt_reg = data & 7; // Only the first 3 bits are used
}

void write_watchdog(uint8_t data) {
    /* Space Invaders will call this periodically to reset the watchdog timer.
     * Since our emulation has no use for a watchdog timer, we can ignore it.
     */
}

// This is yucky. Will clean up later. (Which essentially means I'll never clean it up)
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

bool audio_init(void) {
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return false;
    }

    sounds[INVADER_DEATH_SND] = Mix_LoadWAV("../sound/invader_death.wav");
    sounds[FLEET1_SND] = Mix_LoadWAV("../sound/fleet1.wav");
    sounds[FLEET2_SND] = Mix_LoadWAV("../sound/fleet2.wav");
    sounds[FLEET3_SND] = Mix_LoadWAV("../sound/fleet3.wav");
    sounds[FLEET4_SND] = Mix_LoadWAV("../sound/fleet4.wav");
    sounds[PLAYER_DEATH_SND] = Mix_LoadWAV("../sound/player_death.wav");
    sounds[SHOOT_LASER_SND] = Mix_LoadWAV("../sound/shoot_laser.wav");
    sounds[UFO_HIT_SND] = Mix_LoadWAV("../sound/ufo_hit.wav");
    sounds[UFO_MOVE_SND] = Mix_LoadWAV("../sound/ufo_move.wav");

    for (int i = 0; i < NUM_SOUNDS; i++) {
        if (!sounds[i]) {
            fprintf(stderr, "Failed to load sound effect %d\n", i);
        }
    }

    return true;
}

void audio_quit(void) {
    Mix_Quit();
}

bool handle_input() {
    while (SDL_PollEvent(&e)) {
        SDL_Keycode keyc = e.key.keysym.sym;

        switch (e.type) {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            switch (keyc) {
            case SDLK_RETURN:
                inp1_reg |= START_1P;
                break;
            case SDLK_c:
                inp1_reg |= CREDIT;
                break;
            case SDLK_t:
                inp2_reg |= TILT;
                break;
            case SDLK_SPACE:
                inp1_reg |= SHOT_1P;
                inp2_reg |= SHOT_2P;
                break;
            case SDLK_RIGHT:
                inp1_reg |= RIGHT_1P;
                inp2_reg |= RIGHT_2P;
                break;
            case SDLK_LEFT:
                inp1_reg |= LEFT_1P;
                inp2_reg |= LEFT_2P;
                break;
            case SDLK_2:
                inp1_reg |= START_2P;
                break;
            }

            break;
        case SDL_KEYUP:
            switch (keyc) {
            case SDLK_RETURN:
                inp1_reg &= ~START_1P;
                break;
            case SDLK_c:
                inp1_reg &= ~CREDIT;
                break;
            case SDLK_t:
                inp2_reg &= ~TILT;
                break;
            case SDLK_SPACE:
                inp1_reg &= ~SHOT_1P;
                inp2_reg &= ~SHOT_2P;
                break;
            case SDLK_RIGHT:
                inp1_reg &= ~RIGHT_1P;
                inp2_reg &= ~RIGHT_2P;
                break;
            case SDLK_LEFT:
                inp1_reg &= ~LEFT_1P;
                inp2_reg &= ~LEFT_2P;
                break;
            case SDLK_2:
                inp1_reg &= ~START_2P;
                break;
            }

            break;
        }
    }

    return true;
}
