#include "gilberte.h"

#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "game_object.h"
#include "mesh.h"
#include "render.h"
#include "vec.h"

void DrawBackground(void);

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

        SetBuffer(surface->pixels);
    }

    Mesh* mesh = MeshLoadFromObj("assets/sphere_d1.obj");

    Object objects[] = {
        {
            .mesh = mesh,
            .name = "test",
            .position = {.x = 0, .y = 0, .z = 0}
        },
    };

    while (true)
    {
        /* Input processing. */
        {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0)
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                    {
                        goto Exit;
                    } break;
                    case SDL_KEYDOWN:
                    {
                        if (event.key.keysym.sym == SDLK_UP)
                        {
                            objects[0].position.z += 0.5;
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN)
                        {
                            objects[0].position.z -= 0.5;
                        }
                        else if (event.key.keysym.sym == SDLK_LEFT)
                        {
                            objects[0].position.x -= 0.5;
                        }
                        else if (event.key.keysym.sym == SDLK_RIGHT)
                        {
                            objects[0].position.x += 0.5;
                        }
                        else if (event.key.keysym.sym == SDLK_a)
                        {
                            objects[0].position.y += 0.5;
                        }
                        else if (event.key.keysym.sym == SDLK_z)
                        {
                            objects[0].position.y -= 0.5;
                        }
                        else if (event.key.keysym.sym == SDLK_q)
                        {
                            goto Exit;
                        }
                    } break;
                }

            }
        }

        /* Rendering. */
        {
            SDL_LockSurface(surface);
            const Uint64 kBegin = SDL_GetTicks64();

            DrawBackground();
            
            for (size_t i = 0; i < sizeof(objects) / sizeof(Object); ++i)
            {
                RenderObject(&(objects[i]));
            }

            SDL_Log("Scene render time: %llu milliseconds\n",
                    SDL_GetTicks64() - kBegin);
            SDL_UnlockSurface(surface);
            SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);
            SDL_UpdateWindowSurface(window);
        }

        SDL_Delay((Uint32)(1.0 / TARGET_FRAME_RATE * 1000));
    }

    MeshFree(mesh);

Exit:
    {
        SDL_FreeSurface(surface);
        surface = NULL;

        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();

        SDL_Log("Exit with code: [ %d ]\n", exit_code);

        return exit_code;
    }
}

void DrawBackground(void)
{
    for (int y = (WINDOW_HEIGHT / 2); y >= -(WINDOW_HEIGHT / 2); --y)
    {
        for (int x = -(WINDOW_WIDTH / 2); x <= WINDOW_WIDTH / 2; ++x)
        {
            const Vec2_Int p = { .x = x, .y = y };
            DrawPoint(&p, COLOR_BACKGROUND);
        }
    }
}
