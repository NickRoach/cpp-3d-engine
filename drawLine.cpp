#include "math.h"

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

    float x;
    float y;
    for (int v = 0; v < steps; v++)
    {
        x = x + Xincrement;
        y = y + Yincrement;
        if (x >= 0 && x < buffer_width && y >= 0 && y < buffer_height)
        {
            unsigned char *row = (unsigned char *)buffer_memory;
            row += (int)y * buffer_width * sizeof(unsigned int);
            row += (int)x * sizeof(unsigned int);
            unsigned int *pixel = (unsigned int *)row;
            *pixel = color;
        }
    }
}