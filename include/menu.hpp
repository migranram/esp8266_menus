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
    Page * actual_page, * running_page;
    uint8 page_idx;
    std::vector<Page *> pages_list;


    Menu(char *name)
    {
        _name = (char *)malloc(20 * sizeof(char));
        sprintf(_name, name);
        _tft = new TFT_eSPI();

        page_idx = 0;
        actual_page = running_page = nullptr;

        pages_list.push_back(nullptr);

    }

    void addPage(Page *page)
    {
        if (page != nullptr && pages_list.size() < MAX_PAGES)
        {
            pages_list.push_back(page);

            if(actual_page == nullptr)
            {
                actual_page = page;
                page_idx = 1;
            }
        }
    }

    void setup()
    {
        // Setup the TFT display
        // Setup baud rate and draw top banner
        _tft->init();
        _tft->setFreeFont(FF18);
        _tft->setRotation(0); // Must be setRotation(0) for this sketch to work correctly
        _tft->fillScreen(MAIN_COLOR);
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

    void nextPage()
    {
        page_idx++;
        if(page_idx == pages_list.size())
            page_idx = 1;

        actual_page = pages_list[page_idx];
    }
    void previousPage()
    {
        if(page_idx == 1)
            page_idx = pages_list.size() - 1;
        else
            page_idx--;
        
        actual_page = pages_list[page_idx];
    }

private:
};

#endif
