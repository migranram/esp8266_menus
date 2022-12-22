
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

#define MENU_NAME (char *)"test"
#define USB_BAUDRATE 9600

/*

EXAMPLE: CREATES A MENU AND ADDS A PAGE IMPLEMENTING A SERIAL WATCHER.

*/
Menu* myMenu;
SerialPage* p1;
TitledContentPage* p2;


void setup() {
  Serial.begin(USB_BAUDRATE);
  auto wifi_nets = wifi_scan();
  myMenu = new Menu(MENU_NAME);

  p1 = new SerialPage("serial watcher", myMenu->_tft);
  p2 = new TitledContentPage("WiFi Networks", myMenu->_tft);
  p2->setContent(wifi_nets);

  myMenu->addPage(p2);
  myMenu->setup();
}


void loop(void) {
  
  myMenu->display();

}
