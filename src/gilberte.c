#include "gilberte.h"

#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "render.h"
#include "vec3.h"

float sign(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(Vec3 pt, Vec3 v1, Vec3 v2, Vec3 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

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

            Vec3 triangle[] = {
                { .x =  0.0, .y = 0.0, .z = 3.0 },
                { .x = -2.0, .y = 1.0, .z = 5.0 },
                { .x =  1.0, .y = 0.0, .z = 3.0 },
            };

            const double FOV = M_PI / 3.0;
            const double zFar = 1.0;
            const double zNear = 100.0;

            Vec3 triangle_projected[] = {
                { .x = ((double)WINDOW_WIDTH / (double)WINDOW_HEIGHT) / SDL_tan(FOV / 2.0) * triangle[0].x / triangle[0].z,
                  .y = triangle[0].y / SDL_tan(FOV / 2.0) / triangle[0].z,
                  .z = triangle[0].z * ( zFar / (zFar - zNear) ) - ( zFar * zNear / zFar - zNear ) },

                  {.x = ((double)WINDOW_WIDTH / (double)WINDOW_HEIGHT) / SDL_tan(FOV / 2.0) * triangle[1].x / triangle[1].z,
                  .y = triangle[1].y / SDL_tan(FOV / 2.0) / triangle[1].z,
                  .z = triangle[1].z * (zFar / (zFar - zNear)) - (zFar * zNear / zFar - zNear) },

                  {.x = ((double)WINDOW_WIDTH / (double)WINDOW_HEIGHT) / SDL_tan(FOV / 2.0) * triangle[2].x / triangle[2].z,
                  .y = triangle[2].y / SDL_tan(FOV / 2.0) / triangle[2].z,
                  .z = triangle[2].z * (zFar / (zFar - zNear)) - (zFar * zNear / zFar - zNear) },
            };

            for (int i = 0; i < WINDOW_HEIGHT; ++i)
            {
                for (int j = 0; j < WINDOW_WIDTH; ++j)
                {
                    const Vec3 p = {
                        .x = (double)j / WINDOW_WIDTH * 2 - 1,
                        .y = (double)i / WINDOW_HEIGHT * -2 + 1,
                        .z = 0 };

                    if (PointInTriangle(p, triangle_projected[0], triangle_projected[1], triangle_projected[2]))
                    {
                        WriteColor(surface->pixels, j, i, 255, 255, 255);
                    }
                }
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
