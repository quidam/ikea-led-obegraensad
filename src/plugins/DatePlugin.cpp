#include "plugins/DatePlugin.h"

void DatePlugin::setup()
{
  // loading screen
  Screen.setPixel(4, 7, 1);
  Screen.setPixel(5, 7, 1);
  Screen.setPixel(7, 7, 1);
  Screen.setPixel(8, 7, 1);
  Screen.setPixel(10, 7, 1);
  Screen.setPixel(11, 7, 1);
}

void DatePlugin::activate()
{
  previousDay = -1;
  previousMonth = -1;
}

void DatePlugin::loop()
{
  if (getLocalTime(&timeinfo))
  {
    if (previousMonth != timeinfo.tm_mon || previousDay != timeinfo.tm_mday)
    {
      int mon = timeinfo.tm_mon + 1;
      std::vector<int> month = {(mon - mon % 10) / 10, mon % 10};
      std::vector<int> mday = {(timeinfo.tm_mday - timeinfo.tm_mday % 10) / 10, timeinfo.tm_mday % 10};
      Screen.clear();
      Screen.drawBigNumbers(0, 0, mday);
      Screen.drawBigNumbers(0, ROWS / 2, month);
    }

    previousDay = timeinfo.tm_mday;
    previousMonth = timeinfo.tm_mon;
  }
}

const unsigned long DatePlugin::getDuration() const
{
    return 10000uL;
}

const char *DatePlugin::getName() const
{
  return "Date";
}
