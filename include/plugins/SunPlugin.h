#pragma once

#ifdef ESP32
#include <HTTPClient.h>
#endif
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif
#include <ArduinoJson.h>
#include <iostream>
#include <cstring>
#include <string>
#include <time.h>
#include "PluginManager.h"

class SunPlugin : public Plugin
{
private:

// Date
struct tm timeinfo;

// Sunrise / Sunset
  unsigned long lastUpdateSun = 0;
  unsigned int lastUpdateDay = 0;
  unsigned long lastUpdateWaterLevel = 0;
  const String apiStringSunrise = "http://" + String(OPENHAB_SERVER) +":" + String(OPENHAB_PORT) + "/rest/items/" + String(OPENHAB_ITEM_SUNRISE);
  const String apiStringSunset = "http://" + String(OPENHAB_SERVER) +":" + String(OPENHAB_PORT) + "/rest/items/" + String(OPENHAB_ITEM_SUNSET);
  std::string sunrise;
  std::string sunset;
  float waterLevel;

  HTTPClient httpSun;
  HTTPClient httpWaterLevel;

  std::string httpGetSun(String url);
  void draw();

public:
  void updateSun();
  void updateWaterLevel();
  void setup() override;
  void activate() override;
  const char *getName() const override;
};
