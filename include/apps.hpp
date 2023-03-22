#include <pages.hpp>
#include <wifi.h>

/// @brief Example page from the original examples
class SerialPage : public Page
{
public:
    // The initial y coordinate of the top of the scrolling area
    uint16_t yStart = TOP_FIXED_AREA;
    // yArea must be a integral multiple of TEXT_HEIGHT
    uint16_t yArea = YMAX - TOP_FIXED_AREA - BOT_FIXED_AREA;
    // The initial y coordinate of the top of the bottom text line
    uint16_t yDraw = YMAX - BOT_FIXED_AREA - TEXT_HEIGHT;

    // Keep track of the drawing x coordinate
    uint16_t xPos = 0;

    // For the byte we read from the serial port
    byte data = 0;

    // A few test variables used during debugging
    bool change_colour = 1;
    bool selected = 1;
    // We have to blank the top line each time the display is scrolled, but this takes up to 13 milliseconds
    // for a full width line, meanwhile the serial buffer may be filling... and overflowing
    // We can speed up scrolling of short text lines by just blanking the character we drew
    int blank[19]; // We keep all the strings pixel lengths to optimise the speed of the top line blanking

    using Page::Page;

    void setup();

    void display();

private:
    int scroll_line();

    void setupScrollArea(uint16_t tfa, uint16_t bfa);

    void scrollAddress(uint16_t vsp);
};

class WifiPage : public TitledContentPage
{
    using TitledContentPage::TitledContentPage;
    void setup();
    void display();
};


class PageChooser : public TitledContentPage
{

private:
    int selected_option = 0;
    std::vector<Shortcut_widget *> widget_list;
    Shortcut_widget *new_widget;

public:
    PageChooser(const char *name, TFT_eSPI *screen);
    void addOption(uint8_t id);
    void selectOption(int id);
    void nextOption();
    void previousOption();

    void executeOption();

    void display();
};

class OTAUpdate : public TitledContentPage
{};