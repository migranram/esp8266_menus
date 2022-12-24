#include "wifi.h"

// #define PRINT_MAC_ADDR

void wifi_setup()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

void wifi_disconnect()
{
    WiFi.disconnect();
}

String wifi_scan(bool async)
{
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t *bssid;
    int32_t channel;
    bool hidden;
    int scanResult = 0;

    char temp[200];
    auto output = String("");

    scanResult = WiFi.scanNetworks(/*async=*/async, /*hidden=*/true);

    if (scanResult == 0)
    {
        sprintf(temp, PSTR("No networks found"));
    }
    else if (scanResult > 0)
    {
        // Print unsorted scan results
        for (int8_t i = 0; i < scanResult; i++)
        {
            char temp[200];
            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

#ifdef PRINT_MAC_ADDR
            sprintf(temp, PSTR("++%02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n"),
                    i,
                    channel,
                    bssid[0], bssid[1], bssid[2],
                    bssid[3], bssid[4], bssid[5],
                    rssi,
                    (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
                    hidden ? 'H' : 'V',
                    ssid.c_str());
#else

            sprintf(temp, PSTR("++%02d: %ddBm %c %c %s\n"),
                    i,
                    //  channel,
                    rssi,
                    (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
                    hidden ? 'H' : 'V',
                    ssid.c_str());

#endif
            output.concat(temp);
            output.concat("\r");
            delay(0);
        }
    }
    else if (async && scanResult == -1)
    {
        sprintf(temp, "Scanning...");
        output.concat(temp);
    }
    else
    {
        Serial.printf(PSTR("WiFi scan error %d"), scanResult);
        sprintf(temp, PSTR("WiFi scan error %d"), scanResult);
        output.concat(temp);
    }

    return output;
}
