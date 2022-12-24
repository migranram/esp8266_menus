/*

Definitions

*/

// Types of buttons states

enum button_state{
    REST        = 0,
    UP_FLANK    = 1,
    DOWN_FLANK = 2
};

struct button{
    uint8_t pin;
    uint8_t state;
    uint8_t index;
};