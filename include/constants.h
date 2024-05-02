#pragma once

// disable if you do not want to have online functionality
#define ENABLE_SERVER

#ifdef ESP32
#define PIN_ENABLE 26
#define PIN_DATA 27
#define PIN_CLOCK 14
#define PIN_LATCH 12
#define PIN_BUTTON 16
#endif

#ifdef ESP8266
#define PIN_ENABLE 16
#define PIN_DATA 13
#define PIN_CLOCK 14
#define PIN_LATCH 0
#define PIN_BUTTON 2
#endif

#define uS_TO_S_FACTOR 1000000ULL
#define SLEEP_DURATION 7ULL * 60 * 60

#ifdef ENABLE_SERVER
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.json
#define NTP_SERVER "de.pool.ntp.org"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"
#define OPENHAB_SERVER "192.168.178.13"
#define OPENHAB_PORT 8080
#define OPENHAB_ITEM_TEMPERATURE "Heizung_Aussentemperatur"
#define OPENHAB_ITEM_SUNRISE "LokaleSonnendaten_Startzeit"
#define OPENHAB_ITEM_SUNSET "Sonnenuntergang_Startzeit"
#endif

#define COLS 16
#define ROWS 16
#define CANVAS_COLS 144

// set your city or coords (https://github.com/chubin/wttr.in)
#define WEATHER_LOCATION "langenfeld"

#define WATER_LEVEL_UUID "a6ee8177-107b-47dd-bcfd-30960ccc6e9c"

// use ALL of the following to use static IP config
#define IP_ADDRESS "192.168.178.109"
#define SUBNET "255.255.255.0"
#define DNS1 "1.1.1.1"
#define DNS2 "8.8.8.8"
#define GWY "192.168.178.1"

// ---------------

enum SYSTEM_STATUS
{
  NONE,
  WSBINARY,
  UPDATE,
  LOADING,
};

extern SYSTEM_STATUS currentStatus;

enum SCREEN_STATUS
{
  STATIC,
  CRAWLING,
  FINISHED,
};

extern SCREEN_STATUS screenStatus;