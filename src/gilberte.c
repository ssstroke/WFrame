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
    {
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
            for (int y = (WINDOW_HEIGHT / 2); y >= -(WINDOW_HEIGHT / 2); --y)
            {
                for (int x = -(WINDOW_WIDTH / 2); x <= WINDOW_WIDTH / 2; ++x)
                {
                    const Vec2Int p = { .x = x, .y = y };
                    DrawPoint(surface->pixels, &p, COLOR_BACKGROUND);
                }
            }

            /* Draw triangles. */
            //{
            //    const Vec2Int triangles[][3] =
            //    {
            //        {
            //            {.x = -200, .y = -230 },
            //            {.x = 200, .y = 50 },
            //            {.x = 20, .y = 230},
            //        },
            //        {
            //            {.x = -32,   .y = -32   },
            //            {.x = 32,  .y = -32  },
            //            {.x = 20,  .y = 17 },
            //        },
            //    };
            //    for (size_t i = 0; i < (sizeof(triangles) / sizeof(triangles[0])); ++i)
            //    {
            //        /*DrawTriangleSolid(surface->pixels,
            //            &(triangles[i][0]), &(triangles[i][1]), &(triangles[i][2]),
            //            COLOR_FOREGROUND);*/
            //        DrawTriangleShaded(surface->pixels,
            //            &(triangles[i][0]), &(triangles[i][1]), &(triangles[i][2]),
            //            0.0, 1.0, 0.5, COLOR_FOREGROUND);
            //        DrawTriangleWireframe(surface->pixels,
            //            &(triangles[i][0]), &(triangles[i][1]), &(triangles[i][2]),
            //            COLOR_WHITE);
            //    }
            //}

            /* Draw 3D cube. */
            {
                const Vec3 vAf = {
                    .x = -6,
                    .y = -1,
                    .z = 14
                };
                const Vec3 vBf = {
                    .x = -6,
                    .y = 1,
                    .z = 14
                };
                const Vec3 vCf = {
                    .x = -4,
                    .y = 1,
                    .z = 14
                };
                const Vec3 vDf = {
                    .x = -4,
                    .y = -1,
                    .z = 14
                };

                const Vec3 vAb = {
                    .x = -6,
                    .y = -1,
                    .z = 16
                };
                const Vec3 vBb = {
                    .x = -6,
                    .y = 1,
                    .z = 16
                };
                const Vec3 vCb = {
                    .x = -4,
                    .y = 1,
                    .z = 16
                };
                const Vec3 vDb = {
                    .x = -4,
                    .y = -1,
                    .z = 16
                };

                const Vec2Int p0 = ProjectVertex(&vAf);
                const Vec2Int p1 = ProjectVertex(&vBf);
                const Vec2Int p2 = ProjectVertex(&vCf);
                const Vec2Int p3 = ProjectVertex(&vDf);
                const Vec2Int p4 = ProjectVertex(&vAb);
                const Vec2Int p5 = ProjectVertex(&vBb);
                const Vec2Int p6 = ProjectVertex(&vCb);
                const Vec2Int p7 = ProjectVertex(&vDb);

                DrawLine(surface->pixels, &p0, &p1, COLOR_BLACK);
                DrawLine(surface->pixels, &p1, &p2, COLOR_BLACK);
                DrawLine(surface->pixels, &p2, &p3, COLOR_BLACK);
                DrawLine(surface->pixels, &p3, &p0, COLOR_BLACK);

                DrawLine(surface->pixels, &p4, &p5, COLOR_WHITE);
                DrawLine(surface->pixels, &p5, &p6, COLOR_WHITE);
                DrawLine(surface->pixels, &p6, &p7, COLOR_WHITE);
                DrawLine(surface->pixels, &p7, &p4, COLOR_WHITE);

                DrawLine(surface->pixels, &p0, &p4, COLOR_FOREGROUND);
                DrawLine(surface->pixels, &p1, &p5, COLOR_FOREGROUND);
                DrawLine(surface->pixels, &p2, &p6, COLOR_FOREGROUND);
                DrawLine(surface->pixels, &p3, &p7, COLOR_FOREGROUND);
            }

            SDL_Log("Scene render time: %llu milliseconds\n",
                    SDL_GetTicks64() - kBegin);
            SDL_UnlockSurface(surface);
            SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);
            SDL_UpdateWindowSurface(window);
        }

        SDL_Delay((Uint32)(1.0 / TARGET_FRAME_RATE * 1000));
    }

    {
Exit:
        SDL_FreeSurface(surface);
        surface = NULL;

        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();

        SDL_Log("Exit with code: [ %d ]\n", exit_code);

        return exit_code;
    }
}
