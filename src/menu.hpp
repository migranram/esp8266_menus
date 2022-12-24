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

class Page
{
public:
    char *_name;
    TFT_eSPI *_tft;

    Page(const char *name, TFT_eSPI *screen)
    {
        _name = (char *)malloc(20 * sizeof(char));
        sprintf(_name, name);
        _tft = screen;
    }

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
    Page * actual_page, * running_page;
    uint8 page_idx;
    std::vector<Page *> pages_list;


    Menu(char *name);

    void addPage(Page *page);

    void setup();

    void display();

    void nextPage();
    void previousPage();

private:
};

#endif
