#include <SDL.h>

#include <stdbool.h>
#include <stdlib.h>

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

typedef struct {
	double x, y, z;
} Vec3;

typedef enum {
	kErrorInitialization
} ErrorCode;

bool SolveQuadratic(const double a, const double b, const double c, double *x0, double *x1);
Vec3 Normalize(Vec3* vec);
double GetVec3Length(Vec3* vec);
double DotProduct(Vec3* a, Vec3* b);
Vec3 Mult(Vec3* vec, double k);
Vec3 Sub(Vec3* a, Vec3* b);

int main(int argc, char** argv)
{
	int exit_code = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error initializing SDL: %s\n", SDL_GetError());
		exit_code = kErrorInitialization;
		goto Exit;
	}

	SDL_Window* window = SDL_CreateWindow("Gilberte", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error creating window: %s\n", SDL_GetError());
		exit_code = kErrorInitialization;
		goto Exit;
	}

	/*
	* Surface is a plane we draw on.
	* See https://wiki.libsdl.org/SDL2/SDL_Surface
	*/
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
	if (surface == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error creating surface: %s\n", SDL_GetError());
		exit_code = kErrorInitialization;
		goto Exit;
	}

	while (true)
	{
		// Input processing
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

		// Scene rendering
		{
			SDL_LockSurface(surface);

			// Draw background color
			for (int i = 0; i < surface->w * surface->h; ++i)
			{
				Uint32 *pixel = (Uint32*)surface->pixels + i;
				*pixel = SDL_MapRGBA(surface->format, 255, 0, 100, 255);
			}

			// Draw sphere
			// Sphere's origin is at (0, 4, 0) (assuming (x, y, z) ordering) and has a radius of 1
			Vec3 sphere = { .x = 0, .y = 0, .z = 4 };
			double radius = 1;

			Vec3 origin = { 0, 0, 0 };

			for (int i = 0; i < surface->h; ++i) // y
			{
				for (int j = 0; j < surface->w; ++j) // x
				{
					Vec3 ray = { .x = ((double)j / WINDOW_WIDTH * 2 - 1) * WINDOW_WIDTH / WINDOW_HEIGHT,
								 .y = (double)i / WINDOW_HEIGHT * 2 - 1,
								 .z = 1.0};

					Vec3 ray_normalized = Normalize(&ray);

					double a = 1;

					Vec3 b_temp_0 = Mult(&ray_normalized, 2);
					Vec3 b_temp_1 = Sub(&origin, &sphere);
					double b = DotProduct(&b_temp_0, &b_temp_1);

					double c = DotProduct(&b_temp_1, &b_temp_1) - radius * radius;

					double x0, x1;

					if (SolveQuadratic(a, b, c, &x0, &x1) == true)
					{
						Uint32* pixel = (Uint32*)surface->pixels + i * WINDOW_WIDTH + j;
						*pixel = SDL_MapRGBA(surface->format, 128, 128, 128, 255);
					}
				}
			}

			SDL_UnlockSurface(surface);

			SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);

			SDL_UpdateWindowSurface(window);
		
		}
	}

Exit:
	SDL_Quit();

	SDL_Log("Exit with code: [ %d ]\n", exit_code);

	return exit_code;
}

bool SolveQuadratic(const double a, const double b, const double c, double *x0, double *x1)
{
	double discr = b * b - 4 * a * c;

	if (discr < 0)
	{
		return false;
	}
	else if (discr == 0)
	{
		*x0 = *x1 = -0.5 * b / a;
	}
	else
	{
		double q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		*x0 = q / a;
		*x1 = c / q;
	}

	if (x0 > x1)
	{
		double temp = *x0;
		*x0 = *x1;
		*x1 = temp;
	}

	return true;
}

Vec3 Normalize(Vec3* vec)
{
	const double kLength = GetVec3Length(vec);
	Vec3 normalized = { .x = vec->x / kLength, .y = vec->y / kLength, .z = vec->z / kLength };

	return normalized;
}

double GetVec3Length(Vec3 *vec)
{
	return SDL_sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

double DotProduct(Vec3* a, Vec3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vec3 Mult(Vec3* vec, double k)
{
	Vec3 vec_ret = { .x = vec->x * k, .y = vec->y * k, .z = vec->z * k };

	return vec_ret;
}

Vec3 Sub(Vec3* a, Vec3* b)
{
	Vec3 vec_ret = { .x = a->x - b->x, .y = a->y - b->y, .z = a->z - b->z };

	return vec_ret;
}