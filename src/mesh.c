#include "mesh.h"

#include <SDL.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
v 0.000000 3.555570 0.831470
v 0.000000 3.831470 0.555570
v 0.000000 3.980785 0.195090
v 0.000000 4.000000 0.000000
...
...
...
f 479 19 480
f 477 11 12
f 480 20 481
f 477 13 3
*/

Mesh* MeshLoadFromObj(const char* filename)
{
    Mesh* mesh = NULL;

    FILE* file = NULL;
    errno_t error;

    if ((error = fopen_s(&file, filename, "r")) != 0)
    {
        // Your string message can be, at most, 94 characters long. See:
        // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-s-strerror-s-wcserror-s-wcserror-s?view=msvc-170
        char error_message[94];
        strerror_s(error_message, sizeof(error_message), error);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to open '%s': %s\n",
            filename, error_message);
    }
    else
    {
        mesh = SDL_calloc(1, sizeof(Mesh));

        if (mesh == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Memory allocation failed for `%s`\n",
                filename);
        }
        else
        {
            size_t vertex_count = 0;
            size_t triangle_count = 0;

            char line[128];
            while (fgets(line, sizeof(line), file))
            {
                if (line[0] == 'v')
                {
                    do ++vertex_count;
                    while (fgets(line, sizeof(line), file) && line[0] == 'v');
                }
                if (line[0] == 'f')
                {
                    do ++triangle_count;
                    while (fgets(line, sizeof(line), file) && line[0] == 'f');
                }
            }

            mesh->vertices = SDL_malloc(sizeof(Vec3_Double) * vertex_count);
            mesh->triangles = SDL_malloc(sizeof(Vec3_Uint) * triangle_count);
            mesh->triangles_count = triangle_count;

            if (mesh->vertices == NULL || mesh->triangles == NULL)
            {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Memory allocation failed for `%s`\n",
                    filename);
                MeshFree(mesh);
            }
            else
            {
                rewind(file);

                while (fgets(line, sizeof(line), file))
                {
                    if (line[0] == 'v')
                    {
                        for (size_t i = 0; i < vertex_count; ++i)
                        {
                            sscanf_s(line, "v %lf %lf %lf",
                                &(mesh->vertices[i].x),
                                &(mesh->vertices[i].y),
                                &(mesh->vertices[i].z));
                            fgets(line, sizeof(line), file);
                        }
                    }
                    if (line[0] == 'f')
                    {
                        for (size_t i = 0; i < triangle_count; ++i)
                        {
                            sscanf_s(line, "f %lu %lu %lu",
                                &(mesh->triangles[i].x),
                                &(mesh->triangles[i].y),
                                &(mesh->triangles[i].z));

                            // Account for 1-based indexing is .obj files.
                            --mesh->triangles[i].x;
                            --mesh->triangles[i].y;
                            --mesh->triangles[i].z;

                            fgets(line, sizeof(line), file);
                        }
                    }
                }
            }
        }
    }

    return mesh;
}

void MeshFree(Mesh* mesh)
{
    free(mesh->vertices);
    free(mesh->triangles);
    free(mesh);
}
