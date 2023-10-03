#include "typedefs.h"
#include "constants.h"
#include "limit.h"

void moveSquare(float &x, float &y, Input input, float dt)
{
    if (input.buttons[BUTTON_UP].is_down)
    {
        y = (y + (float)squareSpeed * dt);
    }
    else if (input.buttons[BUTTON_DOWN].is_down)
    {
        y = y - (float)squareSpeed * dt;
    }
    if (input.buttons[BUTTON_LEFT].is_down)
    {
        x = x - (float)squareSpeed * dt;
    }
    else if (input.buttons[BUTTON_RIGHT].is_down)
    {
        x = x + (float)squareSpeed * dt;
    }
}