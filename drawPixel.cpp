void drawPixel(void *buffer_memory, int buffer_width, int buffer_height, int x, int y, int color)
{
    unsigned int *pixel = (unsigned int *)buffer_memory;
    pixel += x + y * buffer_width;
    *pixel = color;
}
