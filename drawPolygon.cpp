#include "typedefs.h"
#include "drawLine.h"

void drawPolygon(void *buffer_memory, int buffer_width, int buffer_height, Coord coords[], int coordCount, int color)
{
    for (int i = 0; i < coordCount; i++)
    {
        drawLine(buffer_memory, buffer_width, buffer_height, coords[i].x, coords[i].y, coords[(i + 1) % coordCount].x, coords[(i + 1) % coordCount].y, color);
    }
}
