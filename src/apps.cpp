#include <apps.hpp>
#include <wifi.h>

void SerialPage::setup()
{
    // Setup scroll area
    setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);

    _tft->fillScreen(TFT_BLACK);

    _tft->setTextColor(TFT_WHITE, MAIN_COLOR);
    _tft->fillRect(0, 0, 240, 16, MAIN_COLOR);

    // char *string;
    // string = (char *)malloc(50*sizeof(char));
    // sprintf(string,  "Test Terminal - %d baud", (int)baudrate );
    _tft->drawCentreString(_name, 120, 0, 2);
    // free(string);

    // Change colour for scrolling zone text
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);

    // Zero the array
    for (byte i = 0; i < 18; i++)
        blank[i] = 0;

    xPos = 0;
    yDraw = YMAX - BOT_FIXED_AREA - TEXT_HEIGHT;
    yStart = TOP_FIXED_AREA;
}

void SerialPage::display()
{
    while (Serial.available())
    {
        data = Serial.read();
        // If it is a CR or we are near end of line then scroll one line
        if (data == '\r' || xPos > 231)
        {
            xPos = 0;
            yDraw = scroll_line(); // It can take 13ms to scroll and blank 16 pixel lines
        }
        if (data > 31 && data < 128)
        {
            xPos += _tft->drawChar(data, xPos, yDraw, 2);
            blank[(18 + (yStart - TOP_FIXED_AREA) / TEXT_HEIGHT) % 19] = xPos; // Keep a record of line lengths
        }
        // change_colour = 1; // Line to indicate buffer is being emptied
    }
}

int SerialPage::scroll_line()
{
    int yTemp = yStart; // Store the old yStart, this is where we draw the next line
    // Use the record of line lengths to optimise the rectangle size we need to erase the top line
    _tft->fillRect(0, yStart, blank[(yStart - TOP_FIXED_AREA) / TEXT_HEIGHT], TEXT_HEIGHT, TFT_BLACK);

    // Change the top of the scroll area
    yStart += TEXT_HEIGHT;
    // The value must wrap around as the screen memory is a circular buffer
    if (yStart >= YMAX - BOT_FIXED_AREA)
        yStart = TOP_FIXED_AREA + (yStart - YMAX + BOT_FIXED_AREA);
    // Now we can scroll the display
    scrollAddress(yStart);
    return yTemp;
}

void SerialPage::setupScrollArea(uint16_t tfa, uint16_t bfa)
{
    _tft->writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
    _tft->writedata(tfa >> 8);           // Top Fixed Area line count
    _tft->writedata(tfa);
    _tft->writedata((YMAX - tfa - bfa) >> 8); // Vertical Scrolling Area line count
    _tft->writedata(YMAX - tfa - bfa);
    _tft->writedata(bfa >> 8); // Bottom Fixed Area line count
    _tft->writedata(bfa);
}

void SerialPage::scrollAddress(uint16_t vsp)
{
    _tft->writecommand(ILI9341_VSCRSADD); // Vertical scrolling pointer
    _tft->writedata(vsp >> 8);
    _tft->writedata(vsp);
}

void WifiPage::setup()
{
    setContent(wifi_scan(false));
    TitledContentPage::setup();
}

void WifiPage::display()
{
    return;
}

PageChooser::PageChooser(const char *name, TFT_eSPI *screen) : TitledContentPage(name, screen) {}

void PageChooser::addOption(uint8_t id)
{
    new_widget = new Shortcut_widget(this, parent_menu, id);
    widget_list.push_back(new_widget);
}

void PageChooser::selectOption(int id)
{
    if (id >= 0 && id < widget_list.size())
        selected_option = id;
    Serial.printf("Selected option: %d\n", widget_list[selected_option]->page_idx);
}

void PageChooser::nextOption()
{
    if (selected_option + 1 < widget_list.size())
        selectOption(selected_option + 1);
    else
        selectOption(0);
}

void PageChooser::previousOption()
{
    if (selected_option - 1 >= 0)
        selectOption(selected_option - 1);
    else
        selectOption(widget_list.size() - 1);
}

void PageChooser::executeOption()
{
    widget_list[selected_option]->onClick();
}

void PageChooser::display()
{
    for (int i = 0; i < widget_list.size(); ++i)
    {
        int color = i == selected_option ? SEC_COLOR : MAIN_COLOR;
        widget_list[i]->display(20, 80 + 30 * i, 200, 25, color);
    }
}