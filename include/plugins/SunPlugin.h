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

// Sunrise / Sunset
  unsigned long lastUpdate = 0;
  std::string sunrise;
  std::string sunset;

  HTTPClient http;

  void draw();

public:
  void update();
  void setup() override;
  void activate() override;
  void loop() override;
  const unsigned long getDuration() const override;
  const char *getName() const override;
};
