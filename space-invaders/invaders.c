#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "hardware.h"

// Map IO ports to CPU IO memory
void port_init(CPU *cpu) {
    cpu->input[INP1] = read_inp1;
    cpu->input[INP2] = read_inp2;

    cpu->input[SHFT_IN] = read_shift;
    cpu->output[SHFT_DATA] = write_shift;
    cpu->output[SHFTAMNT] = write_shift_amnt;

    cpu->output[SOUND1] = write_snd1;
    cpu->output[SOUND2] = write_snd2;

    cpu->output[WATCHDOG] = write_watchdog;
}

// Initializes SDL
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

// Create the SDL window
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

int main(void) {
    CPU cpu;
    cpu_init(&cpu);
    cpu_load_rom(&cpu, "../roms/space-invaders/invaders.bin", 0x0000);
    port_init(&cpu);

    init_SDL();
    audio_init();
    SDL_Window *window = create_window();
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Event e;

    while(!cpu.exit) {
        // Draw the screen and handle input
        display_draw(window, surface, &cpu);
        cpu.exit = !handle_input(&e);

        uint32_t ticks = SDL_GetTicks();

        // Execute all cycles before a half-screen refresh
        while (cpu.total_cycles < (VBLANK_RATE / 2) + 1) {
            cpu_tick(&cpu);
        }
        cpu_req_interrupt(&cpu, OP_RST_1);

        // Execute all cycles before a full-screen refresh
        while (cpu.total_cycles < VBLANK_RATE) {
            cpu_tick(&cpu);
        }
        cpu_req_interrupt(&cpu, OP_RST_2);

        // Reset cycle counter and sleep until end of refresh period
        cpu.total_cycles = 0;
        SDL_Delay((1000 / REFRESH_RATE) - (SDL_GetTicks() - ticks));
    }

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    audio_quit();
    SDL_Quit();
}
