/*

GPIP functions

*/

#include "gpio.h"

GPIO_handler::GPIO_handler()
{
}

void GPIO_handler::addInput(uint8_t pin)
{
    input_devices.push_back(Input_Device(pin));
    
}

void GPIO_handler::setup()
{
    for (std::vector<Input_Device>::iterator it = input_devices.begin();
         it != input_devices.end();
         it++)
    {
        pinMode(it->pin, INPUT_PULLUP);
        it->state = input_device_state::REST;
    }
}

void GPIO_handler::checkButtons()
{
    for (unsigned int i = 0; i < input_devices.size(); i++)
    {
        input_devices[i].checkState();
    }
}

input_device_state GPIO_handler::getInputState(uint8_t pin)
{
    unsigned int i;
    for (i = 0; i < input_devices.size() && input_devices[i].pin != pin; i++);
    return input_devices[i].state;
}