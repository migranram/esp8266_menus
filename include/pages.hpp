#ifndef PAGES_H
#define PAGES_H
#include <menu.hpp>

class TitledContentPage : public Page
{
    public:
    // The initial y coordinate of the top of the scrolling area
    uint16_t yStart = TOP_FIXED_AREA;
    // The initial y coordinate of the top of the bottom text line
    uint16_t yDraw = YMAX - BOT_FIXED_AREA - TEXT_HEIGHT;

    // Keep track of the drawing x coordinate
    uint16_t xPos = 0;

    // For the byte we read from the serial port
    byte data = 0;

    String content, last_content;

    TitledContentPage(const char *name, TFT_eSPI *screen);

    void setContent(String cont);

    void draw_text();

    void setup();
    void display();

};

class Widget
{
    public:
    const Page *parent_page;
    Menu *menu;
    bool selected = false;

    Widget(Page *fp, Menu *m);

    virtual void onClick();
    virtual void display();

};

class Shortcut_widget : public Widget
{
    public:
    const uint8_t page_idx;
    Shortcut_widget(Page *fp, Menu *m, const uint8_t id);

    void onClick();
    void display(int posx, int posy, int width, int height, int color);
};  

#endif