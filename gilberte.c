#include "gilberte.h"

#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "render.h"
#include "vec3.h"

int main(int argc, char** argv)
{
    int exit_code = kErrorOk;

    /* Initialization. */
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error initializing SDL: %s\n",
                     SDL_GetError());
        exit_code = kErrorInitialization;
        goto Exit;
    }

    window = SDL_CreateWindow("Gilberte",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error creating window: %s\n",
                     SDL_GetError());
        exit_code = kErrorInitialization;
        goto Exit;
    }

    surface = SDL_CreateRGBSurfaceWithFormat(0, WINDOW_WIDTH, WINDOW_HEIGHT,
                                             32, SDL_PIXELFORMAT_RGBA32);
    if (surface == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error creating surface: %s\n",
                     SDL_GetError());
        exit_code = kErrorInitialization;
        goto Exit;
    }

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                goto Exit;
            }
        }

        /* Try some pixel drawing. */
        {
            SDL_LockSurface(surface);

            /* Fill background. */
            for (int y = 0; y < WINDOW_HEIGHT; ++y)
            {
                for (int x = 0; x < WINDOW_WIDTH; ++x)
                {
                    WriteColor(surface->pixels, x, y, rand() % 256, rand() % 256, rand() % 256);
                }
            }

            SDL_UnlockSurface(surface);

            SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);

            SDL_UpdateWindowSurface(window);
        }
    }

Exit:
    SDL_FreeSurface(surface);
    surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    SDL_Log("Exit with code: [ %d ]\n", exit_code);

    return exit_code;
}
