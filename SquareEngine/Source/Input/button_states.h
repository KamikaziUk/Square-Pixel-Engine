#pragma once
#ifndef BUTTONSTATES
#define BUTTONSTATES

enum class ButtonState
{
    None,
    Down,
    Held,
    Up
};

void UpdateButtonState(ButtonState* state, bool pressed);

#endif