#ifndef WIFI_LIB
#define WIFI_LIB

#include "ESP8266WiFi.h"

void wifi_setup();

void wifi_disconnect();

String wifi_scan(bool async);
#endif