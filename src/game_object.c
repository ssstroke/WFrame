#include "game_object.h"

#include "gilberte.h"

#include "render.h"
#include "vec.h"

void RenderObject(const Object* object)
{
    //  For every triangle {
    //      Translate each vertex according to object's position
    //      Project it's three vertices
    //      Draw triangle out of projected vertices
    //  }

    for (size_t i = 0; i < object->mesh->triangles_count; ++i)
    {
        const Vec3_Double translated_A = Vec3Add(
            &(object->mesh->vertices[object->mesh->triangles[i].x]),
            &(object->position));
        const Vec3_Double translated_B = Vec3Add(
            &(object->mesh->vertices[object->mesh->triangles[i].y]),
            &(object->position));
        const Vec3_Double translated_C = Vec3Add(
            &(object->mesh->vertices[object->mesh->triangles[i].z]),
            &(object->position));

        const Vec2_Int projected_A = ProjectVertex(&translated_A);
        const Vec2_Int projected_B = ProjectVertex(&translated_B);
        const Vec2_Int projected_C = ProjectVertex(&translated_C);

        DrawTriangleWireframe(&projected_A, &projected_B, &projected_C,
            COLOR_FOREGROUND);
    }
}
