
/*************************************************************
  This sketch implements a simple serial receive terminal
  program for monitoring serial debug messages from another
  board.
  
  Connect GND to target board GND
  Connect RX line to TX line of target board
  Make sure the target and terminal have the same baud rate
  and serial stettings!

  The sketch works with the ILI9341 TFT 240x320 display and
  the called up libraries.
  
  The sketch uses the hardware scrolling feature of the
  display. Modification of this sketch may lead to problems
  unless the ILI9341 data sheet has been understood!

  Updated by Bodmer 21/12/16 for TFT_eSPI library:
  https://github.com/Bodmer/TFT_eSPI
  
  BSD license applies, all text above must be included in any
  redistribution
 *************************************************************/
#include "menu.hpp"

#define MENU_NAME "test"
#define USB_BAUDRATE 9600

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

    void setup()
    {
        // Setup scroll area
        setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);

        _tft->fillScreen(TFT_BLACK);

        _tft->setTextColor(TFT_WHITE, TFT_BLUE);
        _tft->fillRect(0, 0, 240, 16, TFT_BLUE);

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
    }

    void display()
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

private:
    int scroll_line()
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

    void setupScrollArea(uint16_t tfa, uint16_t bfa)
    {
        _tft->writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
        _tft->writedata(tfa >> 8);           // Top Fixed Area line count
        _tft->writedata(tfa);
        _tft->writedata((YMAX - tfa - bfa) >> 8); // Vertical Scrolling Area line count
        _tft->writedata(YMAX - tfa - bfa);
        _tft->writedata(bfa >> 8); // Bottom Fixed Area line count
        _tft->writedata(bfa);
    }

    void scrollAddress(uint16_t vsp)
    {
        _tft->writecommand(ILI9341_VSCRSADD); // Vertical scrolling pointer
        _tft->writedata(vsp >> 8);
        _tft->writedata(vsp);
    }
};

Menu* myMenu;
SerialPage* p;


void setup() {
  myMenu = new Menu(MENU_NAME);
  p = new SerialPage("serial watcher", myMenu->_tft);
  myMenu->addPage(p);
  myMenu->setup(USB_BAUDRATE);

}


void loop(void) {
  
  myMenu->display();

}
