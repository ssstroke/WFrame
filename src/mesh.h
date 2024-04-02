#pragma once

#include "vec.h"

/*
* - `vertices` is a pointer to Vec3_Double elements that represent points in space,
*   e.g. [ (-1.0, 5.1, 6.7), (1.0, 1.7, 2.3), (-4.0, 5.5, 9.7), ... ].
* 
* - `triangles` is a pointer to Vec3_Int elements, each one representing three indices
*   of `vertices` array, i.e. one element represents a three points of a triangle,
*   e.g. [ (0, 1, 2), (0, 2, 3), (4, 0, 3), ... ]
*    
*/
typedef struct {
    Vec3_Double* vertices;
    Vec3_Uint* triangles;
    size_t triangles_count;
} Mesh;

Mesh* MeshLoadFromObj(const char* filename);

void MeshFree(Mesh* mesh);
