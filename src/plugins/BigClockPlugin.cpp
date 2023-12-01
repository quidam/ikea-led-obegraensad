#include "plugins/BigClockPlugin.h"

void BigClockPlugin::setup()
{
  // loading screen
  Screen.setPixel(4, 7, 1);
  Screen.setPixel(5, 7, 1);
  Screen.setPixel(7, 7, 1);
  Screen.setPixel(8, 7, 1);
  Screen.setPixel(10, 7, 1);
  Screen.setPixel(11, 7, 1);
}

void BigClockPlugin::activate()
{
  Screen.clear();

  if (getLocalTime(&timeinfo)) {
    std::vector<int> hh = {(timeinfo.tm_hour - timeinfo.tm_hour % 10) / 10, timeinfo.tm_hour % 10};
    std::vector<int> mm = {(timeinfo.tm_min - timeinfo.tm_min % 10) / 10, timeinfo.tm_min % 10};
    Screen.drawBigNumbers(0, 0, hh);
    Screen.drawBigNumbers(0, ROWS / 2, mm);
  }

  Screen.switchScreen(16);
}

const char *BigClockPlugin::getName() const
{
  return "Big Clock";
}
