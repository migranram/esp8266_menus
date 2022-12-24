/*

GPIP functions

*/
#ifndef GPIO_H
#define GPIO_H
#include <stdio.h>
#include <iostream/iostream.h>
#include "definitions.hpp"

enum input_device_state
{
    UNDEFINED = -1,
    REST = 0,
    UP_FLANK = 1,
    DOWN_FLANK = 2
};

enum gpio_type
{
    INPUT_T = 0,
    OUTPUT_T = 1
};

class Device_GPIO
{
public:
    Device_GPIO(uint8_t p, gpio_type t) : pin(p), type(t) {}

public:
    const uint8_t pin;
    const gpio_type type;
};

class Input_Device : public Device_GPIO
{
public:
    input_device_state state;
    uint8_t new_value;
    uint8_t old_value;

    Input_Device(uint8_t p) : Device_GPIO(p, gpio_type::INPUT_T)
    {
        state = input_device_state::UNDEFINED;
        new_value = old_value = 0;
    }
    void checkValue()
    {
        new_value = digitalRead(pin);
    }

    void checkState()
    {
        this->checkValue();
        if (new_value == old_value)
            state = input_device_state::REST;
        else if (new_value != old_value)
        {
            if (new_value)
                state = input_device_state::DOWN_FLANK;
            else
                state = input_device_state::UP_FLANK;
        }

        old_value = new_value;
    }
};

class GPIO_handler
{
public:
    std::vector<uint8> state;
    std::vector<Input_Device> input_devices;

    GPIO_handler();

    void setup();

    void checkButtons();

    void addInput(uint8_t pin);

    input_device_state getInputState(uint8_t pin);

private:
};

#endif