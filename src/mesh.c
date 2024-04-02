// TODO: Check for memory leaks.

#include "mesh.h"

#include <SDL.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

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
        mesh = (Mesh*)malloc(sizeof(Mesh));

        if (mesh == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Memory allocation failed for `%s`\n",
                filename);
        }
        else
        {
            mesh->vertices = NULL;
            mesh->triangles = NULL;

            size_t vertex_count = 0;
            size_t triangles_count = 0;

            char line[128];
            while (fgets(line, sizeof(line), file))
            {
                if (line[0] == 'v')
                {
                    Vec3_Double* previous_vertices = mesh->vertices;
                    mesh->vertices = (Vec3_Double*)realloc(mesh->vertices,
                        (vertex_count + 1) * sizeof(Vec3_Double));

                    if (mesh->vertices == NULL)
                    {
                        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Memory allocation failed for `%s`\n",
                            filename);

                        free(previous_vertices);
                        MeshFree(mesh);
                        
                        break;
                    }
                    else
                    {
                        sscanf_s(line, "v %lf %lf %lf",
                            &((mesh->vertices + vertex_count)->x),
                            &((mesh->vertices + vertex_count)->y), 
                            &((mesh->vertices + vertex_count)->z));

                        // .obj files I load come from Blender, where positive X goes to the left
                        // (although in editor it shows positive X to the right... idk.)
                        (mesh->vertices + vertex_count)->x *= -1;

                        ++vertex_count;
                    }
                }
                else if (line[0] == 'f')
                {
                    Vec3_Uint* previous_triangles = mesh->triangles;
                    mesh->triangles = (Vec3_Uint*)realloc(mesh->triangles,
                        (triangles_count + 1) * sizeof(Vec3_Uint));

                    if (mesh->triangles == NULL)
                    {
                        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Memory allocation failed for `%s`\n",
                            filename);

                        free(previous_triangles);
                        MeshFree(mesh);

                        break;
                    }
                    else
                    {
                        sscanf_s(line, "f %d %d %d",
                            &((mesh->triangles + triangles_count)->x),
                            &((mesh->triangles + triangles_count)->y),
                            &((mesh->triangles + triangles_count)->z));
                        --(mesh->triangles + triangles_count)->x;
                        --(mesh->triangles + triangles_count)->y;
                        --(mesh->triangles + triangles_count)->z;
                        ++triangles_count;
                    }
                }
            }
        }

        fclose(file);
    }

    return mesh;
}

void MeshFree(Mesh* mesh)
{
    free(mesh->vertices);
    free(mesh->triangles);
    free(mesh);
}
