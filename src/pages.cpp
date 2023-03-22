#include "menu.hpp"
#include <pages.hpp>

/*

This software uses part of the TFT_eSPI library examples source code (TFT_Terminal).
Below its BSD License.

*/
TitledContentPage::TitledContentPage(const char *name, TFT_eSPI *screen) : Page(name, screen) {}

void TitledContentPage::setContent(String cont)
{
    content = cont;
}

void TitledContentPage::draw_text()
{
    last_content = content;
    xPos = 0;
    yDraw = yStart;
    for (char *current_char = content.begin(); current_char != content.end(); current_char++)
    {
        data = *current_char;
        // If it is a CR or we are near end of line then scroll one line
        if (data == '\r' || xPos > 231 || data == '\t')
        {
            xPos = 0;
            // yDraw = scroll_line(); // It can take 13ms to scroll and blank 16 pixel lines
            yDraw += _tft->fontHeight(2);
        }
        if (data > 31 && data < 128)
        {
            xPos += _tft->drawChar(data, xPos, yDraw, 2);
        }
        // change_colour = 1; // Line to indicate buffer is being emptied
    }
}

void TitledContentPage::setup()
{
    _tft->fillScreen(TFT_BLACK);

    _tft->setTextColor(TFT_WHITE, MAIN_COLOR);
    _tft->fillRect(0, 0, 240, 16, MAIN_COLOR);

    _tft->drawCentreString(_name, 120, 0, 2);

    _tft->setTextColor(TFT_WHITE, TFT_BLACK);

    draw_text();
}

void TitledContentPage::display()
{
    if (content != last_content)
    {
        setup();
    }
}

Widget::Widget(Page *fp, Menu *m)
{
    parent_page = fp;
    menu = m;
}

void Widget::onClick() {}

void Widget::display() {}


Shortcut_widget::Shortcut_widget(Page *fp, Menu *m, const uint8_t id) : Widget::Widget(fp, m), page_idx(id) {}
void Shortcut_widget::display(int posx, int posy, int width, int height, int color)
{
    menu->_tft->drawRoundRect(posx, posy, width, height, 3, color);
    menu->_tft->setTextColor(TFT_WHITE, color);
    menu->_tft->drawCentreString(String(menu->pages_list[page_idx]->_name), posx + width / 2, posy - menu->_tft->fontHeight(2) / 2 + height / 2, 2);
}
void Shortcut_widget::onClick()
{
    this->menu->gotoPage(page_idx);
}