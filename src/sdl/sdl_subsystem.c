#include "engine_subsystems.h"
#include "engine_screen.h"
#include "engine_logic.h"
#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
} SdlDisplay;

SdlDisplay display;

Uint32 to_sdl_colformat(const colformat colorFormat);
void reset_pulse_keys(input_state* const state);
void update_controls(input_state* const state, SDL_Event* const e);

int init_render_subsystem(
	const screen_format* const format,
	screen_buffer* const screen)
{
    display = (SdlDisplay)
    {
        .window = NULL,
        .renderer = NULL,
        .texture = NULL
    };

    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        return -1;
    }

    display.window =
        SDL_CreateWindow(
            "Draw Test",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            format->width,
            format->height,
            SDL_WINDOW_SHOWN
        );

    if (display.window == NULL)
    {
        return -1;
    }

    display.renderer =
        SDL_CreateRenderer(
            display.window,
            -1,
            SDL_RENDERER_SOFTWARE
        );

    if (display.renderer == NULL)
    {
        return -1;
    }

    display.texture =
        SDL_CreateTexture(
            display.renderer,
            to_sdl_colformat(format->format),
            SDL_TEXTUREACCESS_STREAMING,
            format->width,
            format->height
        );

    if (display.texture == NULL)
    {
        return -1;
    }

    screen->width = format->width;
    screen->height = format->height;
    screen->bitsPP = to_bpp(format->format);
    screen->bytesPP = screen->bitsPP / 8;
    screen->sizeInPixels = format->width * format->height;
    screen->stride = screen->width * screen->bytesPP;
    screen->sizeInBytes = screen->stride * screen->height;
    screen->colorFormat = format->format;

    screen->pixels = malloc(screen->sizeInBytes);
    if (screen->pixels == NULL)
    {
        fprintf(stderr, "Failed to allocate screen pixel buffer\n");
        return -1;
    }

    return 0;
}

int destroy_render_subsystem(screen_buffer* const screen)
{
    free(screen->pixels);
    SDL_DestroyTexture(display.texture);
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();

    return 0;
}

int render_screen(screen_buffer* const screen)
{
    int res = SDL_RenderClear(display.renderer);
    if (res)
    {
        return -1;
    }

    uint8_t* texturePixels = NULL;
    int pitch = -1;

    res =
        SDL_LockTexture(
            display.texture,
            NULL,
            (void**)&texturePixels,
            &pitch
        );

    if (res)
    {
        return -1;
    }

    memcpy(texturePixels, screen->pixels, pitch * screen->height);

    SDL_UnlockTexture(display.texture);

    res =
        SDL_RenderCopy(
            display.renderer,
            display.texture,
            NULL,
            NULL
        );

    if (res)
    {
        return -1;
    }

    SDL_RenderPresent(display.renderer);

    return 0;
}

int init_input_subsystem()
{
    return 0;
}

int destroy_input_subsystem()
{
    return 0;
}

int update_input_state(input_state* const state)
{
    SDL_Event e;
    reset_pulse_keys(state);

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            state->quit = true;

            return;
        }
        else
        {
            update_controls(state, &e);
        }
    }

    return 0;
}

Uint32 to_sdl_colformat(const colformat colorFormat)
{
    switch (colorFormat)
    {
    case CF_ARGB:
        return SDL_PIXELFORMAT_ARGB8888;
    case CF_RGBA:
        return SDL_PIXELFORMAT_RGBA8888;
    case CF_RGB565:
        return SDL_PIXELFORMAT_RGB565;
    default:
        return SDL_PIXELFORMAT_RGBA8888;
    }
}

void reset_pulse_keys(input_state* const state)
{
    state->toggleDebug = false;
    state->toggleRenderMode = false;
}

void update_controls(input_state* const state, SDL_Event* const e)
{
    if (e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_ESCAPE:
            state->quit = true;
            return;
        case SDLK_v:
            state->toggleDebug = true;
            break;
        case SDLK_b:
            state->toggleRenderMode = true;
            break;
        case SDLK_w:
            state->forwards = true;
            break;
        case SDLK_s:
            state->backwards = true;
            break;
        case SDLK_d:
            if (e->key.keysym.mod == KMOD_LCTRL)
            {
                state->rotRight = false;
                state->right = true;
            }
            else
            {
                state->rotRight = true;
                state->right = false;
            }
            break;
        case SDLK_a:
            if (e->key.keysym.mod == KMOD_LCTRL)
            {
                state->rotLeft = false;
                state->left = true;
            }
            else
            {
                state->rotLeft = true;
                state->left = false;
            }
            break;
        }
    }
    else if (e->type == SDL_KEYUP)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_w:
            state->forwards = false;
            break;
        case SDLK_s:
            state->backwards = false;
            break;
        case SDLK_d:
            state->rotRight = false;
            state->right = false;
            break;
        case SDLK_a:
            state->rotLeft = false;
            state->left = false;
            break;
        }
    }
}
