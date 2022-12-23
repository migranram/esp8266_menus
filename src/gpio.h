/*

GPIP functions

*/

#include <stdio.h>
#include <iostream/iostream.h>
#include "definitions.hpp"

class GPIO_handler
{
public:
    std::vector<uint8> input_pins;
    std::vector<uint8> output_pins;
    std::vector<uint8> new_value;
    std::vector<uint8> old_value;
    std::vector<uint8> state;

    GPIO_handler(std::vector<uint8> in_pins, std::vector<uint8> out_pins);

    std::vector<uint8> setup();

    void checkButtons();

    void getState();

private:
};