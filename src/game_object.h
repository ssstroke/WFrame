// TOOD: Seems much better to use `Vec3_Double* position` instead of `Vec3_Double position`
//       as it would reduce struct size. But I will keep it like this for now.

#pragma once

#include "mesh.h"
#include "vec.h"

typedef struct {
    char* name;
    Mesh* mesh;
    Vec3_Double position;
} Object;

void RenderObject(const Object*);
