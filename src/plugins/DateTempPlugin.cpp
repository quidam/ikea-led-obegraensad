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

    // Screen.drawNumbers(0, 2, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
    // Screen.drawNumbers(8, 2, {(month - month % 10) / 10, month % 10});
    // Screen.setPixel(7, 6, 1, 30);
    // Screen.setPixel(15, 6, 1, 30);
    Screen.drawBigNumbers(0, 4, {(dayOfMonth - dayOfMonth % 10) / 10, dayOfMonth % 10});
    Screen.setPixel(17, 9, 1);
    Screen.setPixel(18, 9, 1);
    Screen.setPixel(17, 10, 1);
    Screen.setPixel(18, 10, 1);
    Screen.drawBigNumbers(19, 4, {(month - month % 10) / 10, month % 10});
    Screen.setPixel(36, 9, 1);
    Screen.setPixel(37, 9, 1);
    Screen.setPixel(36, 10, 1);
    Screen.setPixel(37, 10, 1);

    uint8_t canvasCols = 38;

    Screen.switchScreen(canvasCols);
}

const char *DateTempPlugin::getName() const
{
    return "DateTemp";
}
