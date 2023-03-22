/* Miguel Granero Ramos 2023 

BSD 3-Clause License

*/
#include <iostream/iostream.h>

#include "menu.hpp"
#include "apps.hpp"
#include "gpio.h"
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

DisplayTask display_task;
WifiScanTask wifiscan_task;
GPIOScan gpioscan_task;


static void main_loop()
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
      myMenu->gotoPage(p3->page_index);
    }
    if (gpio_handler->getInputState(PIN_D2) == input_device_state::UP_FLANK)
    {
      p2->setup();
    }
  }
  else
  {
    if (gpio_handler->getInputState(PIN_D4) == input_device_state::UP_FLANK)
    {
      myMenu->gotoPage(p3->page_index);
    }
    if (gpio_handler->getInputState(PIN_D2) == input_device_state::UP_FLANK)
    {
      myMenu->gotoPage(p3->page_index);
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