#include "typedefs.h"
#include "drawLine.h"
#include "drawPixel.h"
#include "math.h"

void fillTriangle(void *buffer_memory, int buffer_width, int buffer_height, Triangle tri, int color)
{
    // sort the vertices by y value
    if (tri.p[0].y > tri.p[1].y)
    {
        Vec3d temp = tri.p[0];
        tri.p[0] = tri.p[1];
        tri.p[1] = temp;
    }
    if (tri.p[1].y > tri.p[2].y)
    {
        Vec3d temp = tri.p[1];
        tri.p[1] = tri.p[2];
        tri.p[2] = temp;
    }
    if (tri.p[0].y > tri.p[1].y)
    {
        Vec3d temp = tri.p[0];
        tri.p[0] = tri.p[1];
        tri.p[1] = temp;
    }

    // dx, dy 0,1
    float dx0 = tri.p[1].x - tri.p[0].x;
    float dy0 = tri.p[1].y - tri.p[0].y;
    int steps01 = abs(dy0);
    float Xincrement01 = dx0 / (float)steps01;

    // dx, dy 1,2
    float dx1 = tri.p[2].x - tri.p[1].x;
    float dy1 = tri.p[2].y - tri.p[1].y;
    int steps12 = abs(dy1);
    float Xincrement12 = dx1 / (float)steps12;

    // dx, dy 0,2
    float dx2 = tri.p[2].x - tri.p[0].x;
    float dy2 = tri.p[2].y - tri.p[0].y;
    int steps02 = abs(dy2);
    float Xincrement02 = dx2 / (float)steps02;

    // 0, 1: red
    // drawLine(buffer_memory, buffer_width, buffer_height, tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, 0xff0000);

    // // 1, 2: green
    // drawLine(buffer_memory, buffer_width, buffer_height, tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y, 0x00ff00);

    // // 0, 2: blue
    // drawLine(buffer_memory, buffer_width, buffer_height, tri.p[0].x, tri.p[0].y, tri.p[2].x, tri.p[2].y, 0x0000ff);

    // draw bottom half of triangle
    for (int i = 0; i <= steps01; i++)
    {
        float x = tri.p[0].x + Xincrement01 * i;
        float y = tri.p[0].y + i;
        drawLine(buffer_memory, buffer_width, buffer_height, x - 1, y, x + (Xincrement02 - Xincrement01) * i, y, color);
    }

    // draw top half of triangle
    for (int i = 0; i <= steps12; i++)
    {
        float x = tri.p[2].x - Xincrement12 * i;
        float y = tri.p[2].y - i;
        drawLine(buffer_memory, buffer_width, buffer_height, x - 1, y, x - (Xincrement02 - Xincrement12) * i, y, color);
    }

    // drawLine(buffer_memory, buffer_width, buffer_height, tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, 0xff0000);
    // drawLine(buffer_memory, buffer_width, buffer_height, tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y, 0x00ff00);

    // drawLine(buffer_memory, buffer_width, buffer_height, tri.p[2].x, tri.p[2].y, tri.p[0].x, tri.p[0].y, 0x0000ff);
    // blue is the uninterrupted side
}