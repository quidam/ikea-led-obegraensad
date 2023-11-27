#include "plugins/DateTempPlugin.h"

#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void DateTempPlugin::setup()
{
    // loading screen
    Screen.clear();
    currentStatus = LOADING;
    Screen.setPixel(4, 7, 1);
    Screen.setPixel(5, 7, 1);
    Screen.setPixel(7, 7, 1);
    Screen.setPixel(8, 7, 1);
    Screen.setPixel(10, 7, 1);
    Screen.setPixel(11, 7, 1);
    currentStatus = NONE;
}

void DateTempPlugin::activate() {
    dayOfMonth = 0;
    month = 0;
    draw();
}

void DateTempPlugin::loop()
{
    if (getLocalTime(&timeinfo))
    {
        if (month != timeinfo.tm_mon + 1 || dayOfMonth != timeinfo.tm_mday)
        {
            dayOfMonth = timeinfo.tm_mday;
            month = timeinfo.tm_mon + 1;
            draw();
        }
    }
}

void DateTempPlugin::draw()
{
    Screen.clear();

    // Date

    Screen.drawBigNumbers(4, 4, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
    Screen.setPixel(21, 9, 1);
    Screen.setPixel(22, 9, 1);
    Screen.setPixel(21, 10, 1);
    Screen.setPixel(22, 10, 1);
    Screen.drawBigNumbers(23, 4, {(month - month % 10) / 10, month % 10});
    Screen.setPixel(40, 9, 1);
    Screen.setPixel(41, 9, 1);
    Screen.setPixel(40, 10, 1);
    Screen.setPixel(41, 10, 1);

    Screen.switchScreen(46);
}

const char *DateTempPlugin::getName() const
{
    return "DateTemp";
}
