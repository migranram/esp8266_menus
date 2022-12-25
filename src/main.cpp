
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
#include "tasks.hpp"

#define MENU_NAME (char *)"test"
#define USB_BAUDRATE 9600

/*

EXAMPLE: CREATES A MENU AND ADDS A PAGE IMPLEMENTING A SERIAL WATCHER.

*/
Menu *myMenu;
SerialPage *p1;
TitledContentPage *p2;
PageChooser *p3;

GPIO_handler *gpio_handler;

unsigned long actual_millis;
unsigned long last_millis;


void main_loop()
{

    // Check IO
  if (myMenu->page_idx == p3->page_index)
  {
    if (gpio_handler->getInputState(PIN_D4) == input_device_state::UP_FLANK)
    {
      p3->nextOption();
    }
    if (gpio_handler->getInputState(PIN_D2) == input_device_state::UP_FLANK)
    {
      p3->executeOption();
    }
  }
  else if (myMenu->page_idx == p2->page_index)
  {
    if (gpio_handler->getInputState(PIN_D4) == input_device_state::UP_FLANK)
    {
      p2->setup();
    }
    if (gpio_handler->getInputState(PIN_D2) == input_device_state::UP_FLANK)
    {
      myMenu->nextPage();
    }
  }
  else
  {
    if (gpio_handler->getInputState(PIN_D4) == input_device_state::UP_FLANK)
    {
      myMenu->previousPage();
    }
    if (gpio_handler->getInputState(PIN_D2) == input_device_state::UP_FLANK)
    {
      myMenu->nextPage();
    }
  }

  // Update wifi list
  p2->setContent(wifiscan_task.getScan());

}

class MainTask : public LeanTask
{

public:
    void loop()
    {
        main_loop();
        delay(50);
    }
} main_task;

void setup()
{
  // GPIO
  gpio_handler = new GPIO_handler();
  gpio_handler->addInput(PIN_D4);
  gpio_handler->addInput(PIN_D2);

  gpio_handler->setup();

  // Serial
  Serial.begin(USB_BAUDRATE);

  // Wifi and menu set-up
  myMenu = new Menu(MENU_NAME);
  myMenu->setup();

  p1 = new SerialPage((char *)"Serial watcher", myMenu->_tft);
  p2 = new WifiPage((char *)"WiFi Networks", myMenu->_tft);
  p3 = new PageChooser((char *)"Main Menu", myMenu->_tft);

  int i3 = myMenu->addPage(p3);
  int i1 = myMenu->addPage(p1);
  int i2 = myMenu->addPage(p2);


  p3->addOption(i1);
  p3->addOption(i2);
  p3->addOption(i3);

  // Additional
  actual_millis = last_millis = millis();

  // Tasks
  display_task.setMenu(myMenu);
  Scheduler.start(&display_task);
  //Scheduler.start(&wifiscan_task);

  gpioscan_task.setHandler(gpio_handler);
  Scheduler.start(&gpioscan_task);
  Scheduler.start(&main_task);

  Scheduler.begin();
}

void loop(){}