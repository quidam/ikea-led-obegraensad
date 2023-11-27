#include <Arduino.h>
#include <SPI.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include "PluginManager.h"

#ifdef ENABLE_SERVER
#include "plugins/BigClockPlugin.h"
#include "plugins/DateTempPlugin.h"
#include "plugins/SunPlugin.h"
#include "plugins/WeatherPlugin.h"
#endif

#include "websocket.h"
#include "secrets.h"
#include "ota.h"
#include "webserver.h"
#include "screen.h"

PluginManager pluginManager;
SYSTEM_STATUS currentStatus = NONE;
SCREEN_STATUS screenStatus = STATIC;

unsigned long lastConnectionAttempt = 0;
const unsigned long connectionInterval = 10000;

void connectToWiFi()
{
  Serial.begin(115200);
  Serial.println("Connecting to Wi-Fi...");

  // Delete old config
  WiFi.disconnect(true);

  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  // Check connection result
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect to Wi-Fi. Please check credentials.");
  }

  lastConnectionAttempt = millis();
}

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

// server
#ifdef ENABLE_SERVER
  connectToWiFi();

  // set time server
  configTzTime(TZ_INFO, NTP_SERVER);

  initOTA(server);
  initWebsocketServer(server);
  initWebServer();
#endif

  Screen.setup();

#ifdef ENABLE_SERVER
  Plugin *bigClockPlugin = new BigClockPlugin();
  Plugin *dateTempPlugin = new DateTempPlugin();
  Plugin *sunPlugin = new SunPlugin();
  Plugin *weatherPlugin = new WeatherPlugin();
  int bigClockPluginId = pluginManager.addPlugin(bigClockPlugin);
  int dateTempPluginId = pluginManager.addPlugin(dateTempPlugin);
  int sunPluginId = pluginManager.addPlugin(sunPlugin);
  int weatherPluginId = pluginManager.addPlugin(weatherPlugin);

  pluginManager.addScheduleItem(bigClockPluginId, 5);
  pluginManager.addScheduleItem(dateTempPluginId, 0);
  // pluginManager.addScheduleItem(bigClockPluginId, 5);
  // pluginManager.addScheduleItem(weatherPluginId, 10);
  pluginManager.addScheduleItem(bigClockPluginId, 5);
  pluginManager.addScheduleItem(sunPluginId, 0);
#endif

  pluginManager.init();
}

void loop()
{
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    if (timeinfo.tm_hour >= 23 ) {
    // if (timeinfo.tm_sec == 0 ) {
      esp_sleep_enable_timer_wakeup(SLEEP_DURATION * uS_TO_S_FACTOR);
      esp_deep_sleep_start();
    }
  }

  pluginManager.runActivePlugin();

  if (WiFi.status() != WL_CONNECTED && millis() - lastConnectionAttempt > connectionInterval)
  {
    Serial.println("Lost connection to Wi-Fi. Reconnecting...");
    connectToWiFi();
  }

#ifdef ENABLE_SERVER
  cleanUpClients();
#endif

  delay(1);
}