#include "math.h"
#include "drawPixel.h"

void drawLine(
    void *buffer_memory,
    int buffer_width,
    int buffer_height,
    int x0,
    int y0,
    int x1,
    int y1,
    unsigned int color)
{
    float x = x0;
    float y = y0;
    int steps;
    int dx = x1 - x0;
    int dy = y1 - y0;
    if (abs(dx) > abs(dy))
    {
        steps = abs(dx);
    }
    else
    {
        steps = abs(dy);
    }
    float Xincrement = dx / (float)steps;
    float Yincrement = dy / (float)steps;
    for (int v = 0; v < steps; v++)
    {
        x = x + Xincrement;
        y = y + Yincrement;
        if (x >= 0 && x < buffer_width && y >= 0 && y < buffer_height)
        {
            drawPixel(buffer_memory, buffer_width, buffer_height, x, y, color);
            drawPixel(buffer_memory, buffer_width, buffer_height, x + 1, y, color);
            drawPixel(buffer_memory, buffer_width, buffer_height, x, y + 1, color);
        }
    }
}