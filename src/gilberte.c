#include "gilberte.h"

#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "render.h"
#include "vec.h"

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
        /* Input processing. */
        {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    goto Exit;
                }
            }
        }

        /* Rendering. */
        {
            SDL_LockSurface(surface);

            const Uint64 kBegin = SDL_GetTicks64();

            /* Draw background. */
            for (int y = 0; y < WINDOW_HEIGHT; ++y)
            {
                for (int x = 0; x < WINDOW_WIDTH; ++x)
                {
                    const Vec2Int p = { .x = x, .y = y };
                    DrawPoint(surface->pixels, &p, COLOR_BACKGROUND);
                }
            }

            const Vec2Int a = { .x = 32,
                                .y = 420 };
            const Vec2Int b = { .x = 256,
                                .y = 16 };
            const Vec2Int c = { .x = 500,
                                .y = 150 };

            DrawTriangleFilled(surface->pixels, &a, &b, &c, COLOR_FOREGROUND);
            DrawTriangleWireframe(surface->pixels, &a, &b, &c, 0, 0, 0);

            SDL_Log("Scene render time: %llu milliseconds\n",
                    SDL_GetTicks64() - kBegin);

            SDL_UnlockSurface(surface);

            SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);

            SDL_UpdateWindowSurface(window);
        }

        SDL_Delay(1.0 / TARGET_FRAME_RATE * 1000);
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
