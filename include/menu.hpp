#ifndef MENU_H
#define MENU_H

/*
Class for the menu
*/
#include <stdio.h>
#include <iostream/iostream.h>
#include <TFT_eSPI.h>
#include "fonts.hpp"

#define NAME_CHARS 20

// The scrolling area must be a integral multiple of TEXT_HEIGHT
#define TEXT_HEIGHT 16    // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0  // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 16 // Number of lines in top fixed area (lines counted from top of screen)
#define YMAX 320          // Bottom of screen area
#define MAX_PAGES 10
#define MAIN_COLOR TFT_RED
#define SEC_COLOR TFT_BLUE

class AbstractPage
{
public:
    char *_name;
    TFT_eSPI *_tft;
    uint8_t page_index;

    virtual void setParent(void *) {}
    virtual void setIndex(uint8_t id) {}
    virtual void setup()
    {
    }

    virtual void display()
    {
    }
};

class Menu
{
public:
    char *_name;

    TFT_eSPI *_tft;
    AbstractPage *actual_page, *running_page;
    uint8 page_idx;
    std::vector<AbstractPage *> pages_list;

    Menu(char *name);

    uint8_t addPage(AbstractPage *page);

    void setup();

    void display();

    void nextPage();
    void previousPage();
    void gotoPage(uint8 id);

private:
};

class Page : public AbstractPage
{
public:
    Page(const char *name, TFT_eSPI *screen)
    {
        _name = (char *)malloc(20 * sizeof(char));
        sprintf(_name, name);
        _tft = screen;
    }

    Menu *parent_menu;

    void setParent(void *menu)
    {
        parent_menu = (Menu *)menu;
    }
    void setIndex(uint8_t id)
    {
        page_index = id;
    }
};

#endif
