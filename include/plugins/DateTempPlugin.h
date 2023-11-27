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
#include "PluginManager.h"

class DateTempPlugin : public Plugin
{
private:

// Date
  struct tm timeinfo;

  int dayOfMonth;
  int month;

  void draw();

public:
  void setup() override;
  void activate() override;
  void loop() override;
  const char *getName() const override;
};
