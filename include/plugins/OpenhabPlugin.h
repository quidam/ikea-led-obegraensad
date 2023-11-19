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

class OpenhabPlugin : public Plugin
{
private:
  unsigned long lastUpdate = 0;
  std::string value;

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
