#include <tasks.hpp>
#include <menu.hpp>
#include <wifi.h>
#include <gpio.h>

/*

List of tasks:
    1. Display task.
    2. WIfi Scan.
    3. GPIO check


The void loop() is called at the beginning of each iteration by the scheduler so it could be used.
But according to the documentations, it is recommended not to use it.

*/

void DisplayTask::setMenu(Menu * menu)
{
    if (menu != nullptr)
        myMenu = menu;
}
void DisplayTask::loop()
{
    myMenu->display();
    delay(10);
}

void WifiScanTask::loop()
{
    output = wifi_scan(false);
    
    delay(10e3);
}

String WifiScanTask::getScan()
{
    return this->output;
}

void GPIOScan::setHandler(GPIO_handler * gpio)
{
    if(gpio != nullptr)
        handler = gpio;
}
void GPIOScan::loop()
{
    
    handler->checkButtons();
    delay(50);
}