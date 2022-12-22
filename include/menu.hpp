#ifndef MENU_H
#define MENU_H

/*
Class for the menu
*/
#include <stdio.h>
#include <iostream/iostream.h>
#include <TFT_eSPI.h>

#define NAME_CHARS 20

// The scrolling area must be a integral multiple of TEXT_HEIGHT
#define TEXT_HEIGHT 16    // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0  // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 16 // Number of lines in top fixed area (lines counted from top of screen)
#define YMAX 320          // Bottom of screen area
#define MAX_PAGES 10

class Page
{
public:
    char *_name;
    TFT_eSPI *_tft;

    Page(char *name, TFT_eSPI *screen)
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
    int _baudrate;
    Page * actual_page, * running_page;
    uint8 page_idx;
    Page ** pages_list;

    Menu(char *name)
    {
        _name = (char *)malloc(20 * sizeof(char));
        sprintf(_name, name);
        _tft = new TFT_eSPI();
        _baudrate = -1;

        page_idx = 0;
        actual_page = running_page = nullptr;
        pages_list = (Page **) malloc(MAX_PAGES * sizeof(Page *));
        for (int i = 0; i < MAX_PAGES; i++)
        {
            pages_list[i] = nullptr;
        }
    }

    void addPage(Page *page)
    {
        if (page != nullptr)
        {
            pages_list[page_idx] = page;
            page_idx++;
            actual_page = page;
        }
    }

    void setup(int baudrate)
    {
        // Setup the TFT display
        // Setup baud rate and draw top banner
        _baudrate = baudrate;
        Serial.begin(baudrate);
        _tft->init();
        _tft->setRotation(0); // Must be setRotation(0) for this sketch to work correctly
        _tft->fillScreen(TFT_BLUE);
    }

    void display()
    {
        if(actual_page != nullptr)
        {
            if(actual_page != running_page)
            {
                actual_page->setup();
                running_page = actual_page;
            }

            actual_page->display();
        }
            
    }

private:
};

#endif
