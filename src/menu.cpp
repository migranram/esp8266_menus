
#include "menu.hpp"

Menu::Menu(char *name)
{
    _name = (char *)malloc(20 * sizeof(char));
    sprintf(_name, name);
    _tft = new TFT_eSPI();

    page_idx = 0;
    actual_page = running_page = nullptr;

    pages_list.push_back(nullptr);
}

void Menu::addPage(Page *page)
{
    if (page != nullptr && pages_list.size() < MAX_PAGES)
    {
        pages_list.push_back(page);

        if (actual_page == nullptr)
        {
            actual_page = page;
            page_idx = 1;
        }
    }
}

void Menu::setup()
{
    // Setup the TFT display
    // Setup baud rate and draw top banner
    _tft->init();
    _tft->setFreeFont(FF18);
    _tft->setRotation(0); // Must be setRotation(0) for this sketch to work correctly
    _tft->fillScreen(MAIN_COLOR);
}

void Menu::display()
{
    if (actual_page != nullptr)
    {
        if (actual_page != running_page)
        {
            actual_page->setup();
            running_page = actual_page;
        }

        actual_page->display();
    }
}

void Menu::nextPage()
{
    page_idx++;
    if (page_idx == pages_list.size())
        page_idx = 1;

    actual_page = pages_list[page_idx];
}
void Menu::previousPage()
{
    if (page_idx == 1)
        page_idx = pages_list.size() - 1;
    else
        page_idx--;

    actual_page = pages_list[page_idx];
}
