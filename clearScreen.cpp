void clearScreen (
    void* buffer_memory,
    int buffer_width,
    int buffer_height,
    unsigned int color
) {
    unsigned char* row = (unsigned char*)buffer_memory;
    for (int i = 0; i < buffer_height; i++) {
        unsigned int* pixel = (unsigned int*)row;
        for (int j = 0; j < buffer_width; j++) {
            *pixel++ = color;
        }
        row += buffer_width * sizeof(unsigned int);
    }
}