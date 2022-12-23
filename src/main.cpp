
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
#include "wifi.h"
#include "pages.hpp"
#include "gpio.h"
#include <iostream/iostream.h>

#define MENU_NAME (char *)"test"
#define USB_BAUDRATE 9600

/*

EXAMPLE: CREATES A MENU AND ADDS A PAGE IMPLEMENTING A SERIAL WATCHER.

*/
Menu *myMenu;
SerialPage *p1;
TitledContentPage *p2;

GPIO_handler * gpio_handler;
std::vector<uint8> registered_input_pins = {PIN_D4};
std::vector<uint8> registered_output_pins = {};
std::vector<uint8> buttons_state;

unsigned long actual_millis;
unsigned long last_millis;


void setup()
{
  // GPIO
  gpio_handler = new GPIO_handler(registered_input_pins,registered_output_pins);
  gpio_handler->setup();

  actual_millis = last_millis = millis();

  // Serial
  Serial.begin(USB_BAUDRATE);

  // Wifi and menu set-up
  auto wifi_nets = wifi_scan(false);
  myMenu = new Menu(MENU_NAME);

  p1 = new SerialPage("Serial watcher", myMenu->_tft);
  p2 = new TitledContentPage("WiFi Networks", myMenu->_tft);
  p2->setContent(wifi_nets);

  myMenu->addPage(p1);
  myMenu->addPage(p2);
  myMenu->setup();
}

void loop(void)
{
  // Check IO
  bool scan_wifi = false;
  actual_millis = millis();
  // if(actual_millis - last_millis > 15000)
  // {
  //   p2->setContent(String("Scanning..."));
  //   scan_wifi = true;
  //   last_millis = millis();
  // }

  gpio_handler->getState();
  if (gpio_handler->state[0] == button_state::UP_FLANK){
    myMenu->nextPage();
    Serial.printf("PP: %d", myMenu->page_idx);
  }
  myMenu->display();

  // if(scan_wifi)
  // {
  //   auto wifi_nets = wifi_scan(false);
  //   p2->setContent(wifi_nets);
  // }
}
