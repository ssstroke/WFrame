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

            const Vec2Int triangles[][3] =
            {
                {
                    { .x = 32,  .y = 420 },
                    { .x = 256, .y = 16  },
                    { .x = 480, .y = 150 },
                },
                {
                    { .x = 500, .y = 420 },
                    { .x = 550, .y = 32  },
                    { .x = 500, .y = 64  },
                }
            };

            for (size_t i = 0; i < (sizeof(triangles) / sizeof(triangles[0])); ++i)
            {
                /*DrawTriangleSolid(surface->pixels,
                    &(triangles[i][0]), &(triangles[i][1]), &(triangles[i][2]),
                    COLOR_FOREGROUND);*/
                DrawTriangleShaded(surface->pixels,
                    &(triangles[i][0]), &(triangles[i][1]), &(triangles[i][2]),
                    0.0, 1.0, 0.5, COLOR_FOREGROUND);
                DrawTriangleWireframe(surface->pixels,
                    &(triangles[i][0]), &(triangles[i][1]), &(triangles[i][2]),
                    COLOR_WHITE);
            }

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
