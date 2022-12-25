/*

TASKS DEFINITIONS

*/

#ifndef TASKS_H
#define TASKS_H

#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <menu.hpp>

#include <wifi.h>
#include <gpio.h>

/*

List of tasks:
    1. Display task.
    2. GPIO tasks


The void loop() is called at the beginning of each iteration by the scheduler so it could be used.
But according to the documentations, it is recommended not to use it.

*/

class DisplayTask : public LeanTask
{
private:
    Menu *myMenu;

public:
    void setMenu(Menu * menu)
    {
        if (menu != nullptr)
            myMenu = menu;
    }
    void loop()
    {
        myMenu->display();
        delay(0);
    }
} display_task;

class WifiScanTask : public LeanTask
{
private:
    String output;

public:
    void loop()
    {
        output = wifi_scan(false);
        
        delay(10e3);
    }

    String getScan()
    {
        return this->output;
    }
} wifiscan_task;

class GPIOScan : public LeanTask
{
private:
    GPIO_handler * handler;

public:
    void setHandler(GPIO_handler * gpio)
    {
        if(gpio != nullptr)
            handler = gpio;
    }
    void loop()
    {
        
        handler->checkButtons();
        delay(50);
    }
} gpioscan_task;

#endif