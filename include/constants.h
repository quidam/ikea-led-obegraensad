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

// disable if you do not want to use the internal storage
// https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
// timer1 on esp8266 is not compatible with flash file system reads
#ifndef ESP8266
// #define ENABLE_STORAGE
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
#define CANVAS_COLS 120

// set your city or coords (https://github.com/chubin/wttr.in)
#define WEATHER_LOCATION "langenfeld"


// ---------------

enum SYSTEM_STATUS
{
  NONE,
  WSBINARY,
  UPDATE,
  LOADING,
};

extern SYSTEM_STATUS currentStatus;