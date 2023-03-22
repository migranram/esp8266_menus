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
    2. WIfi Scan.
    3. GPIO check


The void loop() is called at the beginning of each iteration by the scheduler so it could be used.
But according to the documentations, it is recommended not to use it.

*/

class DisplayTask : public LeanTask
{
private:
    Menu *myMenu;

public:
    void setMenu(Menu * menu);
    void loop();
};

class WifiScanTask : public LeanTask
{
private:
    String output;

public:
    void loop();
    String getScan();
};

class GPIOScan : public LeanTask
{
private:
    GPIO_handler * handler;

public:
    void setHandler(GPIO_handler * gpio);
    void loop();
};
#endif