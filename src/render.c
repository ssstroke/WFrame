// TODO: Do I really need two separate Lerp functions?
// TODO: Swap in a separate function.
// TODO: Review array sizes for `x012`, `x02`, `h012`, `h02`
//       in DrawTriangle...() functions.

#include "render.h"

#include <SDL.h>

#include "gilberte.h"

void* gBuffer = NULL;

void SetBuffer(void* buffer)
{
    gBuffer = buffer;
}

void LerpInt(const int i0, const double d0, const int i1, const double d1,
    int* values);

void LerpDouble(const int i0, const double d0, const int i1, const double d1,
    double* values);

void DrawPoint(const Vec2_Int* p,
    const int r, const int g, const int b)
{
    const Uint32 x = WINDOW_WIDTH / 2 + p->x;
    const Uint32 y = WINDOW_HEIGHT / 2 - p->y;

    if (x < WINDOW_WIDTH && y < WINDOW_HEIGHT)
    {
        Uint32* pixel = (Uint32*)gBuffer + (y * WINDOW_WIDTH) + x;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        *pixel = (r << 24) | (g << 16) | (b << 8) | 0xFF;
#else
        *pixel = (r) | (g << 8) | (b << 16) | (0xFF << 24);
#endif // SDL_BYTEORDER == SDL_BIG_ENDIAN
    }
}

// TODO: It fucks up the stack.
void DrawLine(
    const Vec2_Int* p0, const Vec2_Int* p1,
    const int r, const int g, const int b)
{
    const Vec2_Int* p0_ = p0;
    const Vec2_Int* p1_ = p1;

    if (abs(p1_->x - p0_->x) > abs(p1_->y - p0_->y))
    {
        // Horizontal-ish line

        if (p0_->x > p1_->x)
        {
            p0_ = p1;
            p1_ = p0;
        }

        int values[WINDOW_WIDTH];

        LerpInt(p0_->x, p0_->y, p1_->x, p1_->y, values);
        for (int x = p0_->x; x <= p1_->x; ++x)
        {
            const Vec2_Int p = { .x = x, .y = values[x - p0_->x] };
            DrawPoint(&p, r, g, b);
            // This is so fucked up. Above function is declared as
            // void DrawPoint(const Vec2Int* p,
            //     const int r, const int g, const int b);
            // I was calling it like DrawPoint(gBuffer, &p, r, g, b);
            // and it was compiling just fine! How the fuck?
        }
    }
    else
    {
        // Vertical-ish line

        if (p0_->y > p1_->y)
        {
            p0_ = p1;
            p1_ = p0;
        }

        int values[WINDOW_HEIGHT];

        LerpInt(p0_->y, p0_->x, p1_->y, p1_->x, values);
        for (int y = p0_->y; y <= p1_->y; ++y)
        {
            const Vec2_Int p = { .x = values[y - p0_->y], .y = y };
            DrawPoint(&p, r, g, b);
        }
    }
}

void DrawTriangleWireframe(
    const Vec2_Int* p0, const Vec2_Int* p1, const Vec2_Int* p2,
    const int r, const int g, const int b)
{
    DrawLine(p0, p1, r, g, b);
    DrawLine(p1, p2, r, g, b);
    DrawLine(p2, p0, r, g, b);
}

void DrawTriangleSolid(
    const Vec2_Int* p0, const Vec2_Int* p1, const Vec2_Int* p2,
    const int r, const int g, const int b)
{
    const Vec2_Int* p0_ = p0;
    const Vec2_Int* p1_ = p1;
    const Vec2_Int* p2_ = p2;

    {
        if (p1_->y < p0_->y)
        {
            // swap p1 and p0
            p0_ = p1;
            p1_ = p0;
        }
        if (p2_->y < p0_->y)
        {
            // swap p2 and p0
            const Vec2_Int* temp = p0_;
            p0_ = p2_;
            p2_ = temp;
        }
        if (p2_->y < p1_->y)
        {
            // swap p2 and p1
            const Vec2_Int* temp = p1_;
            p1_ = p2_;
            p2_ = temp;
        }
    }
    
    {
        int x012[WINDOW_WIDTH];
        int x02[WINDOW_WIDTH]; // it is the "tall" side

        // p1_->y - 1 (i.e. the last point) becase this last point will be x12's first point
        LerpInt(p0_->y, p0_->x, p1_->y, p1_->x, x012);
        LerpInt(p1_->y, p1_->x, p2_->y, p2_->x, x012 + (p1_->y - p0_->y));
        LerpInt(p0_->y, p0_->x, p2_->y, p2_->x, x02);

        int* x_left;
        int* x_right;
        
        const int m = (p2_->y - p0_->y) / 2;
        if (x02[m] < x012[m])
        {
            x_left = x02;
            x_right = x012;
        }
        else
        {
            x_left = x012;
            x_right = x02;
        }

        for (int y = p0_->y; y <= p2_->y; ++y)
        {
            for (int x = x_left[y - p0_->y]; x <= x_right[y - p0_->y]; ++x)
            {
                const Vec2_Int p = { .x = x, .y = y };
                DrawPoint(&p, r, g, b);
            }
        }
    }
}

void DrawTriangleShaded(
    const Vec2_Int* p0, const Vec2_Int* p1, const Vec2_Int* p2,
    double h0, double h1, double h2,
    const int r, const int g, const int b)
{
    const Vec2_Int* p0_ = p0;
    const Vec2_Int* p1_ = p1;
    const Vec2_Int* p2_ = p2;

    {
        if (p1_->y < p0_->y)
        {
            p0_ = p1;
            p1_ = p0;

            double h_temp = h0;
            h0 = h1;
            h1 = h_temp;
        }
        if (p2_->y < p0_->y)
        {
            const Vec2_Int* temp = p0_;
            p0_ = p2_;
            p2_ = temp;

            double h_temp = h0;
            h0 = h2;
            h2 = h_temp;
        }
        if (p2_->y < p1_->y)
        {
            const Vec2_Int* temp = p1_;
            p1_ = p2_;
            p2_ = temp;

            double h_temp = h1;
            h1 = h2;
            h2 = h_temp;
        }
    }

    {
        int x012[WINDOW_WIDTH];
        int x02[WINDOW_WIDTH]; // it is the "tall" side

        double h012[WINDOW_WIDTH];
        double h02[WINDOW_WIDTH];

        // p1_->y - 1 (i.e. the last point) becase this last point will be x12's first point
        LerpInt(p0_->y, p0_->x, p1_->y, p1_->x, x012);
        LerpInt(p1_->y, p1_->x, p2_->y, p2_->x, x012 + (p1_->y - p0_->y));
        LerpInt(p0_->y, p0_->x, p2_->y, p2_->x, x02);

        LerpDouble(p0_->y, h0, p1_->y, h1, h012);
        LerpDouble(p1_->y, h1, p2_->y, h2, h012 + (p1_->y - p0_->y));
        LerpDouble(p0_->y, h0, p2_->y, h2, h02);

        int* x_left;
        int* x_right;

        double* h_left;
        double* h_right;

        const int m = (p2_->y - p0_->y) / 2;
        if (x02[m] < x012[m])
        {
            x_left = x02;
            x_right = x012;

            h_left = h02;
            h_right = h012;
        }
        else
        {
            x_left = x012;
            x_right = x02;

            h_left = h012;
            h_right = h02;
        }

        for (int y = p0_->y; y <= p2_->y; ++y)
        {
            double h_values[WINDOW_WIDTH];
            LerpDouble(x_left[y - p0_->y], h_left[y - p0_->y],
                x_right[y - p0_->y], h_right[y - p0_->y],
                h_values);

            for (int x = x_left[y - p0_->y]; x <= x_right[y - p0_->y]; ++x)
            {
                const Vec2_Int p = { .x = x, .y = y };
                const double h_value = h_values[x - x_left[y - p0_->y]];
                DrawPoint(&p,
                    (int)(h_value * r), (int)(h_value * g), (int)(h_value * b));
            }
        }
    }
}

void LerpInt(const int i0, const double d0, const int i1, const double d1,
    int* values)
{
    if (i0 == i1)
    {
        values[0] = (int)round(d0);
    }

    const double a = (d1 - d0) / (double)(i1 - i0);
    double d = d0;

    for (int i = i0; i <= i1; ++i)
    {
        values[i - i0] = (int)round(d);
        d += a;
    }
}

void LerpDouble(const int i0, const double d0, const int i1, const double d1,
    double* values)
{
    if (i0 == i1)
    {
        values[0] = d0;
    }

    const double a = (d1 - d0) / (double)(i1 - i0);
    double d = d0;

    for (int i = i0; i <= i1; ++i)
    {
        values[i - i0] = d;
        d += a;
    }
}

Vec2_Int ViewportToCanvas(const Vec2_Double* p)
{
    const Vec2_Int result = {
        .x = (int)(p->x * WINDOW_WIDTH / VIEWPORT_WIDTH),
        .y = (int)(p->y * WINDOW_HEIGHT / VIEWPORT_HEIGHT),
    };
    return result;
}

Vec2_Int ProjectVertex(const Vec3_Double* p)
{
    const Vec2_Double projected = {
        .x = p->x * VIEWPORT_LENGTH / p->y,
        .y = p->z * VIEWPORT_LENGTH / p->y * ((double)WINDOW_WIDTH / (double)WINDOW_HEIGHT),
    };
    return ViewportToCanvas(&projected);
}

void RenderMesh(const Mesh* mesh)
{
    //  For every triangle {
    //      Project it's three vertices
    //      Draw triangle out of projected vertices
    //  }

    for (size_t i = 0; i < mesh->triangles_count; ++i)
    {
        // x, y, z of `mesh->triangles[i]` are just
        // 3 indices of 3 vertices of 1 triangle.
        const Vec2_Int projected_A = ProjectVertex(&(mesh->vertices[mesh->triangles[i].x]));
        const Vec2_Int projected_B = ProjectVertex(&(mesh->vertices[mesh->triangles[i].y]));
        const Vec2_Int projected_C = ProjectVertex(&(mesh->vertices[mesh->triangles[i].z]));
        
        DrawTriangleWireframe(&projected_A, &projected_B, &projected_C,
            COLOR_FOREGROUND);
    }
}
