#pragma once

#include "constants.h"

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "screen.h"
#include "PluginManager.h"
#include "secrets.h"
#include "signs.h"

void onOTAStart();
void onOTAEnd();
void initOTA(AsyncWebServer &server);
