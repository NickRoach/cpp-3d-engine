#include "typedefs.h"
#include "constants.h"

void rotateCube(float &x, float &y, Input input, float dt)
{
    if (input.buttons[BUTTON_UP].is_down)
    {
        y = (y + (float)rotateSpeed * dt);
    }
    else if (input.buttons[BUTTON_DOWN].is_down)
    {
        y = y - (float)rotateSpeed * dt;
    }
    if (input.buttons[BUTTON_LEFT].is_down)
    {
        x = x - (float)rotateSpeed * dt;
    }
    else if (input.buttons[BUTTON_RIGHT].is_down)
    {
        x = x + (float)rotateSpeed * dt;
    }
}