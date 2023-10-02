void drawRect(
    void *buffer_memory,
    int buffer_width,
    int buffer_height,
    int x,
    int y,
    int width,
    int height,
    unsigned int color)
{
    unsigned char *row = (unsigned char *)buffer_memory;
    row += y * buffer_width * sizeof(unsigned int);
    row += x * sizeof(unsigned int);

    for (int i = 0; i < height; i++)
    {
        unsigned int *pixel = (unsigned int *)row;
        for (int j = 0; j < width; j++)
        {
            *pixel++ = color;
        }
        row += buffer_width * sizeof(unsigned int);
    }
}