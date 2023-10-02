#include "typedefs.h"
#include "drawLine.h"

void drawTriangle(void *buffer_memory, int buffer_width, int buffer_height, Triangle tri, int color)
{
    // Draw the triangle
    drawLine(buffer_memory, buffer_width, buffer_height, tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, color);
    drawLine(buffer_memory, buffer_width, buffer_height, tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y, color);
    drawLine(buffer_memory, buffer_width, buffer_height, tri.p[2].x, tri.p[2].y, tri.p[0].x, tri.p[0].y, color);
}