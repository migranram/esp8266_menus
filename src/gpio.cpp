/*

GPIP functions

*/

#include "gpio.h"

GPIO_handler::GPIO_handler(std::vector<uint8> in_pins, std::vector<uint8> out_pins) : input_pins(in_pins), output_pins(out_pins)
{
}

std::vector<uint8> GPIO_handler::setup()
{
    for (std::vector<uint8>::iterator it = input_pins.begin();
         it != input_pins.end();
         it++)
    {
        pinMode(*it, INPUT_PULLUP);
        new_value.push_back(0);
        old_value.push_back(0);
        state.push_back(button_state::REST);
    }
    for (std::vector<uint8>::iterator it = output_pins.begin();
         it != output_pins.end();
         it++)
    {
        pinMode(*it, OUTPUT);
    }

    return new_value;
}

void GPIO_handler::checkButtons()
{
    for (int i = 0; i < input_pins.size(); i++)
    {
        new_value[i] = digitalRead(input_pins[i]);
    }
}

void GPIO_handler::getState()
{
    this->checkButtons();
    for (int i = 0; i < new_value.size(); i++)
    {
        if(new_value[i] == old_value[i])
            state[i] = button_state::REST;
        else if(new_value[i] != old_value[i])
        {
            if(new_value[0])
                state[i] = button_state::DOWN_FLANK;
            else
                state[i] = button_state::UP_FLANK;
        }

        old_value[i] = new_value[i];
    }
}