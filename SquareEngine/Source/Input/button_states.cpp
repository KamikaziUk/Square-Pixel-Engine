#include "button_states.h"

namespace SquarePixelEngine
{
    void UpdateButtonState(ButtonState* state, bool pressed)
    {
        if(pressed)
        {
            if(*state == ButtonState::None)
            {
                *state = ButtonState::Down;
            }
            else if(*state == ButtonState::Down)
            {
                *state = ButtonState::Held;
            }
        }
        else
        {
            if(*state == ButtonState::Down ||
                *state == ButtonState::Held)
            {
                *state = ButtonState::Up;
            }
            else if(*state == ButtonState::Up)
            {
                *state = ButtonState::None;
            }
        }
    }
}