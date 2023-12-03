#include "plugins/DatePlugin.h"

#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void DatePlugin::setup()
{
}

void DatePlugin::activate() {
    Screen.clear();

    if (getLocalTime(&timeinfo)) {
        int dayOfMonth = timeinfo.tm_mday;
        int month = timeinfo.tm_mon + 1;
        Screen.drawBigNumbers(4, 4, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
        Screen.setPixel(22, 9, 1);
        Screen.setPixel(23, 9, 1);
        Screen.setPixel(22, 10, 1);
        Screen.setPixel(23, 10, 1);
        Screen.drawBigNumbers(24, 4, {(month - month % 10) / 10, month % 10});
        Screen.setPixel(42, 9, 1);
        Screen.setPixel(43, 9, 1);
        Screen.setPixel(42, 10, 1);
        Screen.setPixel(43, 10, 1);
    }

    Screen.switchScreen(48);
}

const char *DatePlugin::getName() const
{
    return "DateTemp";
}
