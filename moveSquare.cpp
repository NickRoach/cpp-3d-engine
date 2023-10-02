#include "typedefs.h"
#include "constants.h"
#include "limit.h"

void moveSquare(float &x, float &y, Input input, float dt, int buffer_width, int buffer_height)
{
    if (input.buttons[BUTTON_UP].is_down)
    {
        y = limit(y + (float)squareSpeed * dt, 0, buffer_height - squareHeight);
    }
    else if (input.buttons[BUTTON_DOWN].is_down)
    {
        y = limit(y - (float)squareSpeed * dt, 0, buffer_height - squareHeight);
    }
    if (input.buttons[BUTTON_LEFT].is_down)
    {
        x = limit(x - (float)squareSpeed * dt, 0, buffer_width - squareWidth);
    }
    else if (input.buttons[BUTTON_RIGHT].is_down)
    {
        x = limit(x + (float)squareSpeed * dt, 0, buffer_width - squareWidth);
    }
}