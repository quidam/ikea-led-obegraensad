#pragma once

#include "constants.h"

#ifdef ENABLE_SERVER

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;
void initWebServer();

#endif
